#include "protocol_masterslave.hpp"
#include "serialport_readerwriter.hpp"
#include "commands.hpp"

#include <QMetaEnum>

namespace bioconverter {

Protocol_MasterSlave::Protocol_MasterSlave(const std::shared_ptr<SerialPort_ReaderWriter> sp,
										   QObject *parent)
	: QObject(parent),
	  protocolOutput{},
	  m_protocolOutput(&protocolOutput),
	  current_command(CommandName::INVALID),
	  sp(sp)
{
	protocol_commands.emplace(CommandName::GET_SYSTEM_INFO_1, std::make_unique<Get_System_Info_1>());
	protocol_commands.emplace(CommandName::GET_SYSTEM_INFO_2, std::make_unique<Get_System_Info_2>());
	protocol_commands.emplace(CommandName::GET_TAGS_NUMBER_AND_POSITION_0TO5, std::make_unique<Get_Tags_Number_And_Position_0To5>());
	protocol_commands.emplace(CommandName::GET_TAGS_NUMBER_AND_POSITION_6TO11, std::make_unique<Get_Tags_Number_And_Position_6To11>());
	protocol_commands.emplace(CommandName::GET_TAGS_NUMBER_AND_POSITION_12TO17, std::make_unique<Get_Tags_Number_And_Position_12To17>());
	protocol_commands.emplace(CommandName::GET_TAGS_NUMBER_AND_POSITION_18TO23, std::make_unique<Get_Tags_Number_And_Position_18To23>());
	protocol_commands.emplace(CommandName::GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS, std::make_unique<Get_Single_Container_Parameters1_By_Pos>());
	protocol_commands.emplace(CommandName::GET_SINGLE_CONTAINER_PARAMETERS2_BY_POS, std::make_unique<Get_Single_Container_Parameters2_By_Pos>());
	protocol_commands.emplace(CommandName::SET_SINGLE_CONTAINER_PARAMETERS1, std::make_unique<Set_Single_Container_Parameters>());
	protocol_commands.emplace(CommandName::CANCEL_CONTAINER_BY_TAG, std::make_unique<Cancel_Container_By_Tag>());
	protocol_commands.emplace(CommandName::ERASE_EEPROM_RESET_SYSTEM, std::make_unique<Erase_EEPROM_Reset_System>());
	protocol_commands.emplace(CommandName::TRY_TO_INSERT_NEW_CONTAINER, std::make_unique<Try_To_Insert_New_Container>());
	protocol_commands.emplace(CommandName::TRY_TO_SHOW_CONTAINER, std::make_unique<Try_To_Show_Container>());
	protocol_commands.emplace(CommandName::SHOW_CONTAINER_GO_BACK, std::make_unique<Show_Container_Go_Back>());
	protocol_commands.emplace(CommandName::MANAGE_ERROR, std::make_unique<Manage_Error>());
	protocol_commands.emplace(CommandName::DELETE_ALL_ERRORS, std::make_unique<Delete_All_Errors>());
	protocol_commands.emplace(CommandName::ABORT_MANUAL_ACTION, std::make_unique<Abort_Manual_Action>());

	QObject::connect(sp.get(), &SerialPort_ReaderWriter::dataRead,
					 this, &Protocol_MasterSlave::serialDataHandler,
					 Qt::ConnectionType::QueuedConnection);
	QObject::connect(sp.get(), &SerialPort_ReaderWriter::dataError,
					 this, &Protocol_MasterSlave::serialErrorHandler,
					 Qt::ConnectionType::QueuedConnection);
}

Protocol_MasterSlave::~Protocol_MasterSlave()
{

}

void Protocol_MasterSlave::runCommand(const Protocol_MasterSlave::CommandName cmd, const QList<QVariant> &input)
{
	QMetaEnum metaEnum = QMetaEnum::fromType<Protocol_MasterSlave::CommandName>();

	QVariantList output;

	if (current_command != CommandName::INVALID) {
		m_protocolOutput << "The system is executing another command :" << metaEnum.valueToKey(static_cast<int>(current_command))
						 << " when " << metaEnum.valueToKey(static_cast<int>(cmd)) << " was issued\n";
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(QVariant::fromValue(cmd),
							 QVariant::fromValue(MasterError::EXECUTING_OTHER_COMMAND),
							 output, QVariant::fromValue(SlaveError::NO_SLAVE_ERROR));
		return;
	}

	if (!protocol_commands.count(cmd)) {
		m_protocolOutput << "Unsupported command: " << static_cast<unsigned int>(cmd) << "\n";
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(QVariant::fromValue(cmd),
							 QVariant::fromValue(MasterError::UNSUPPORTED_COMMAND),
							 output, QVariant::fromValue(SlaveError::NO_SLAVE_ERROR));
		return;
	}

	QByteArray bytesToBeWriteen;
	bytesToBeWriteen.append(0x55);
	bytesToBeWriteen.append(static_cast<char>(cmd));

	const auto ret_master = protocol_commands.at(cmd)->masterCommand(input, bytesToBeWriteen);
	if (ret_master || bytesToBeWriteen.size() != 9) {
		m_protocolOutput << "Invalid input data for master command: " << metaEnum.valueToKey(static_cast<int>(cmd)) << "\n";
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(QVariant::fromValue(cmd),
							 QVariant::fromValue(MasterError::INVALID_INPUT_DATA),
							 output, QVariant::fromValue(SlaveError::NO_SLAVE_ERROR));
		return;
	}

	std::uint8_t checksum = 0;
	for (const auto b : qAsConst(bytesToBeWriteen)) {
		checksum += b;
	}
	bytesToBeWriteen.append(checksum);

	const auto nbytesWritten = sp->write(bytesToBeWriteen);
	if (nbytesWritten != bytesToBeWriteen.size()) {
		m_protocolOutput << "Failed writing master command: " << metaEnum.valueToKey(static_cast<int>(cmd)) << "\n";
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(QVariant::fromValue(cmd),
							 QVariant::fromValue(MasterError::SERIAL_WRITE_FAILED),
							 output, QVariant::fromValue(SlaveError::NO_SLAVE_ERROR));
		return;
	}

	current_command = cmd;
}

void Protocol_MasterSlave::serialDataHandler(const QByteArray dataRead)
{
	QVariantList output;
	const auto cmd = current_command;
	current_command = CommandName::INVALID;

	if (!protocol_commands.count(cmd)) {
		m_protocolOutput << "Invalid current command type: " << static_cast<int>(cmd) << "\n";
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(QVariant::fromValue(cmd),
							 QVariant::fromValue(MasterError::INVALID_CURRENT_COMMAND),
							 output, QVariant::fromValue(SlaveError::NO_SLAVE_ERROR));
		return;
	}

	QMetaEnum cmdEnum = QMetaEnum::fromType<Protocol_MasterSlave::CommandName>();
	if (!dataRead.size() ||
		dataRead.size() != 10 ||
		dataRead.at(0) != 0x55 ||
		(dataRead.at(1) != static_cast<char>(cmd) &&
		 static_cast<std::uint8_t>(dataRead.at(1)) != 0xFF))
	{
		m_protocolOutput << "Invalid header data received for command: " << cmdEnum.valueToKey(static_cast<int>(cmd)) << "\n";
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(QVariant::fromValue(cmd),
							 QVariant::fromValue(MasterError::INVALID_HEADER_RECEIVED),
							 output, QVariant::fromValue(SlaveError::NO_SLAVE_ERROR));
		return;
	}

	QByteArray bytesRead = dataRead.left(9);
	std::uint8_t checksum = 0;
	for (const auto &b : qAsConst(bytesRead)) {
		checksum += b;
	}
	if (static_cast<std::uint8_t>(dataRead.back()) != checksum) {
		m_protocolOutput << "Invalid checksum data received for command: " << cmdEnum.valueToKey(static_cast<int>(cmd)) << ")\n";
		m_protocolOutput << "(computed:" << Qt::hex << checksum << ",actual:" << Qt::hex << static_cast<std::uint8_t>(dataRead.back()) << ")\n";
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(QVariant::fromValue(cmd),
							 QVariant::fromValue(MasterError::INVALID_CHECKSUM_RECEIVED),
							 output, QVariant::fromValue(SlaveError::NO_SLAVE_ERROR));
		return;
	}

	if (static_cast<std::uint8_t>(dataRead.at(1)) == 0xFF) {
		QMetaEnum errorEnum = QMetaEnum::fromType<SlaveError>();
		m_protocolOutput << "Received protocol error " << errorEnum.valueToKey(static_cast<int>(dataRead.at(2)))
						 << " from slave for command: " << cmdEnum.valueToKey(static_cast<int>(cmd)) << "\n";
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(QVariant::fromValue(cmd),
							 QVariant::fromValue(MasterError::NO_MASTER_ERROR), output,
							 QVariant::fromValue(static_cast<SlaveError>(dataRead.at(2))));
		return;
	}

	bytesRead = bytesRead.right(7);
	const auto ret_slave = protocol_commands.at(cmd)->slaveResponse(bytesRead, output);
	if (ret_slave) {
		m_protocolOutput << "Cannot parse input bytes from slave response of command: " << cmdEnum.valueToKey(static_cast<int>(cmd)) << "\n";
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(QVariant::fromValue(cmd),
							 QVariant::fromValue(MasterError::INVALID_DATA_RECEIVED),
							 output, QVariant::fromValue(SlaveError::NO_SLAVE_ERROR));
		return;
	}

	m_protocolOutput << "Command: " << cmdEnum.valueToKey(static_cast<int>(cmd)) << " executed successfully\n";
	Q_EMIT protocolOutputChanged();
	Q_EMIT commandResult(QVariant::fromValue(cmd),
						 QVariant::fromValue(MasterError::NO_MASTER_ERROR),
						 output, QVariant::fromValue(SlaveError::NO_SLAVE_ERROR));
}

void Protocol_MasterSlave::serialErrorHandler(int error)
{
	m_protocolOutput << "Serial port error: " << error << "\n";
	Q_EMIT protocolOutputChanged();
	const auto cmd = current_command;
	current_command = CommandName::INVALID;
	Q_EMIT commandResult(QVariant::fromValue(cmd),
						 QVariant::fromValue(MasterError::SERIAL_PORT_ERROR),
						 QVariantList(), QVariant::fromValue(SlaveError::NO_SLAVE_ERROR));
}

}
