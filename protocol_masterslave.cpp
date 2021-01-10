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
	  sp(sp)
{
	protocol_commands.emplace(CommandName::GET_SYSTEM_INFO_1, std::make_unique<Get_System_Info_1>());

	QObject::connect(sp.get(), &SerialPort_ReaderWriter::dataRead,
					 this, &Protocol_MasterSlave::serialDataHandler,
					 Qt::ConnectionType::QueuedConnection);
}

Protocol_MasterSlave::~Protocol_MasterSlave()
{

}

void Protocol_MasterSlave::runCommand(const Protocol_MasterSlave::CommandName cmd, const QList<QVariant> &input)
{
	QMetaEnum metaEnum = QMetaEnum::fromType<Protocol_MasterSlave::CommandName>();

	QVariantList output;

	if (!protocol_commands.count(cmd)) {
		m_protocolOutput << "Unsupported command: " << static_cast<unsigned int>(cmd);
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(cmd, -1, output);
		return;
	}

	QByteArray bytesToBeWriteen;
	bytesToBeWriteen.append(0x55);
	bytesToBeWriteen.append(static_cast<char>(cmd));

	const auto ret_master = protocol_commands.at(cmd)->masterCommand(input, bytesToBeWriteen);
	if (ret_master) {
		m_protocolOutput << "Invalid input data for master command: " << metaEnum.valueToKey(static_cast<int>(cmd));
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(cmd, -2, output);
		return;
	}

	std::uint8_t checksum = 0;
	for (const auto b : qAsConst(bytesToBeWriteen)) {
		checksum = checksum ^ b;
	}
	bytesToBeWriteen.append(checksum);

	const auto nbytesWritten = sp->write(bytesToBeWriteen);
	if (nbytesWritten != bytesToBeWriteen.size()) {
		m_protocolOutput << "Failed writing master command: " << metaEnum.valueToKey(static_cast<int>(cmd));
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(cmd, -3, output);
		return;
	}

	current_command = cmd;
}

void Protocol_MasterSlave::serialDataHandler(const QByteArray dataRead)
{
	QVariantList output;

	if (!protocol_commands.count(current_command)) {
		m_protocolOutput << "Invalid current command type: " << static_cast<unsigned int>(current_command);
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(current_command, -4, output);
		current_command = CommandName::INVALID;
		return;
	}

	QMetaEnum metaEnum = QMetaEnum::fromType<Protocol_MasterSlave::CommandName>();
	if (!dataRead.size()) {
		m_protocolOutput << "Empty data received for command: " << metaEnum.valueToKey(static_cast<int>(current_command));
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(current_command, -5, output);
		current_command = CommandName::INVALID;
		return;
	}

	if (dataRead.size() != 10 ||
		dataRead.at(0) != 0x55 ||
		dataRead.at(1) != static_cast<char>(current_command))
	{
		m_protocolOutput << "Invalid header data received for command: " << metaEnum.valueToKey(static_cast<int>(current_command));
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(current_command, -6, output);
		current_command = CommandName::INVALID;
		return;
	}

	std::uint8_t checksum = 0;
	for (const auto b : qAsConst(dataRead)) {
		checksum = checksum ^ b;
	}
	if (dataRead.at(9) != checksum)
	{
		m_protocolOutput << "Invalid checksum data received for command: " << metaEnum.valueToKey(static_cast<int>(current_command));
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(current_command, -7, output);
		current_command = CommandName::INVALID;
		return;
	}

	QByteArray bytesRead = dataRead.mid(2, 7);
	const auto ret_slave = protocol_commands.at(current_command)->slaveResponse(bytesRead, output);
	if (ret_slave) {
		m_protocolOutput << "Invalid input bytes for slave Response of command: " << metaEnum.valueToKey(static_cast<int>(current_command));
		Q_EMIT protocolOutputChanged();
		Q_EMIT commandResult(current_command, -8, output);
		current_command = CommandName::INVALID;
		return;
	}

	Q_EMIT commandResult(current_command, 0, output);
	current_command = CommandName::INVALID;
}

}
