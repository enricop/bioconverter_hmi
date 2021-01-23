#ifndef PROTOCOL_MASTERSLAVE_HPP
#define PROTOCOL_MASTERSLAVE_HPP

#include <QObject>

#include <qqml.h>

#include <map>
#include <memory>

#include <commands.hpp>
#include <protocol_error.hpp>

namespace bioconverter {

class SerialPort_ReaderWriter;

class Protocol_MasterSlave : public QObject
{
	Q_OBJECT

	Q_DISABLE_COPY(Protocol_MasterSlave)

	QML_NAMED_ELEMENT(Protocol_MasterSlave)
	QML_UNCREATABLE("Protocol_MasterSlave is always a named property of backend")

	Q_PROPERTY(QString protocolOutput READ getProtocolOutput NOTIFY protocolOutputChanged)

	Q_PROPERTY(bioconverter::Get_System_Info_1* systeminfo1 READ get1 CONSTANT)
	Q_PROPERTY(bioconverter::Get_System_Info_2* systeminfo2 READ get2 CONSTANT)
	Q_PROPERTY(bioconverter::Get_Tags_Number_And_Position_0To5* tags1 READ tags1 CONSTANT)
	Q_PROPERTY(bioconverter::Get_Tags_Number_And_Position_6To11* tags2 READ tags2 CONSTANT)
	Q_PROPERTY(bioconverter::Get_Tags_Number_And_Position_12To17* tags3 READ tags3 CONSTANT)
	Q_PROPERTY(bioconverter::Get_Tags_Number_And_Position_18To23* tags4 READ tags4 CONSTANT)
	Q_PROPERTY(bioconverter::Get_Single_Container_Parameters1_By_Pos* contp1 READ contp1 CONSTANT)
	Q_PROPERTY(bioconverter::Get_Single_Container_Parameters2_By_Pos* contp2 READ contp2 CONSTANT)
	Q_PROPERTY(bioconverter::Set_Single_Container_Parameters* setp READ setp CONSTANT)

public:
	explicit Protocol_MasterSlave(const std::shared_ptr<SerialPort_ReaderWriter> sp,
								  QObject *parent = nullptr);
	~Protocol_MasterSlave();

	enum class CommandName
	{
		INVALID = 0,
		GET_SYSTEM_INFO_1 = 0x05,
		GET_SYSTEM_INFO_2 = 0x06,
		GET_TAGS_NUMBER_AND_POSITION_0TO5 = 0x08,
		GET_TAGS_NUMBER_AND_POSITION_6TO11 = 0x09,
		GET_TAGS_NUMBER_AND_POSITION_12TO17 = 0x0A,
		GET_TAGS_NUMBER_AND_POSITION_18TO23 = 0x0B,
		GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS = 0x0C,
		GET_SINGLE_CONTAINER_PARAMETERS2_BY_POS = 0x0D,
		SET_SINGLE_CONTAINER_PARAMETERS1 = 0x32,
		CANCEL_CONTAINER_BY_TAG = 0x50,
		ERASE_EEPROM_RESET_SYSTEM = 0x51,
		TRY_TO_INSERT_NEW_CONTAINER = 0x64,
		TRY_TO_SHOW_CONTAINER = 0x65,
		SHOW_CONTAINER_GO_BACK = 0x66,
		MANAGE_ERROR = 0x67,
		DELETE_ALL_ERRORS = 0x68,
	};
	Q_ENUM(CommandName)

	QString getProtocolOutput() {
		return protocolOutput;
	};
	Get_System_Info_1* get1() {
		return static_cast<Get_System_Info_1*>(protocol_commands.at(CommandName::GET_SYSTEM_INFO_1).get());
	};
	Get_System_Info_2* get2() {
		return static_cast<Get_System_Info_2*>(protocol_commands.at(CommandName::GET_SYSTEM_INFO_2).get());
	};
	Get_Tags_Number_And_Position_0To5* tags1() {
		return static_cast<Get_Tags_Number_And_Position_0To5*>(protocol_commands.at(CommandName::GET_TAGS_NUMBER_AND_POSITION_0TO5).get());
	};
	Get_Tags_Number_And_Position_6To11* tags2() {
		return static_cast<Get_Tags_Number_And_Position_6To11*>(protocol_commands.at(CommandName::GET_TAGS_NUMBER_AND_POSITION_6TO11).get());
	};
	Get_Tags_Number_And_Position_12To17* tags3() {
		return static_cast<Get_Tags_Number_And_Position_12To17*>(protocol_commands.at(CommandName::GET_TAGS_NUMBER_AND_POSITION_12TO17).get());
	};
	Get_Tags_Number_And_Position_18To23* tags4() {
		return static_cast<Get_Tags_Number_And_Position_18To23*>(protocol_commands.at(CommandName::GET_TAGS_NUMBER_AND_POSITION_18TO23).get());
	};
	Get_Single_Container_Parameters1_By_Pos* contp1() {
		return static_cast<Get_Single_Container_Parameters1_By_Pos*>(protocol_commands.at(CommandName::GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS).get());
	};
	Get_Single_Container_Parameters2_By_Pos* contp2() {
		return static_cast<Get_Single_Container_Parameters2_By_Pos*>(protocol_commands.at(CommandName::GET_SINGLE_CONTAINER_PARAMETERS2_BY_POS).get());
	};
	Set_Single_Container_Parameters* setp() {
		return static_cast<Set_Single_Container_Parameters*>(protocol_commands.at(CommandName::SET_SINGLE_CONTAINER_PARAMETERS1).get());
	};

public Q_SLOTS:
	void runCommand(const enum CommandName cmd, const QList<QVariant> &input);

private Q_SLOTS:
	void serialDataHandler(const QByteArray dataRead);
	void serialErrorHandler(int error);

Q_SIGNALS:
	void commandResult(const QVariant cmd, const int master_error, const QVariantList proto_output, const QVariant slave_error);

	void protocolOutputChanged();

private:
	QString protocolOutput;
	QTextStream m_protocolOutput;

	CommandName current_command;

	const std::shared_ptr<SerialPort_ReaderWriter> sp;

	std::map<const CommandName, const std::unique_ptr<Command>> protocol_commands;
};

}

#endif // PROTOCOL_MASTERSLAVE_HPP
