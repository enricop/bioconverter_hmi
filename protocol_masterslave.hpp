#ifndef PROTOCOL_MASTERSLAVE_HPP
#define PROTOCOL_MASTERSLAVE_HPP

#include <QObject>

#include <qqml.h>

#include <map>
#include <memory>

#include <commands.hpp>

namespace bioconverter {

class Protocol_MasterSlave : public QObject
{
	Q_OBJECT

	Q_DISABLE_COPY(Protocol_MasterSlave)

	QML_NAMED_ELEMENT(Protocol_MasterSlave)
	QML_UNCREATABLE("Protocol_MasterSlave is always a named property of backend")

	Q_PROPERTY(QString protocolOutput READ getProtocolOutput NOTIFY protocolOutputChanged)

public:
	explicit Protocol_MasterSlave(QObject *parent = nullptr);

	enum class CommandName
	{
		GET_SYSTEM_INFO_1 = 0x05,
		GET_SYSTEM_INFO_2 = 0x06,
		GET_TAGS_NUMBER_AND_POSITION_0TO5 = 0x08,
		GET_TAGS_NUMBER_AND_POSITION_6TO11 = 0x09,
		GET_TAGS_NUMBER_AND_POSITION_12TO17 = 0x0A,
		GET_TAGS_NUMBER_AND_POSITION_18TO23 = 0x0B,
		GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS = 0x0C,
		GET_SINGLE_CONTAINER_PARAMETERS2_BY_POS = 0x0D,
		TRY_TO_INSERT_NEW_CONTAINER = 0x64,
		SET_SINGLE_CONTAINER_PARAMETERS1 = 0x32,
	};

	Q_ENUM(CommandName)

	QString getProtocolOutput() {
		return protocolOutput;
	}

public Q_SLOTS:
	void runCommand(const enum CommandName cmd, const QList<QVariant> &input);

Q_SIGNALS:
	void commandExecuted(const enum CommandName cmd, const int result);

	void protocolOutputChanged();

private:
	QString protocolOutput;
	QTextStream m_protocolOutput;

	std::map<CommandName, std::unique_ptr<Command>> protocol_commands;
};

}

#endif // PROTOCOL_MASTERSLAVE_HPP
