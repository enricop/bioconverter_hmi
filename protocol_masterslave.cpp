#include "protocol_masterslave.hpp"

#include "commands.hpp"

#include <QMetaEnum>

namespace bioconverter {

Protocol_MasterSlave::Protocol_MasterSlave(QObject *parent)
	: QObject(parent),
	  protocolOutput{},
	  m_protocolOutput(&protocolOutput)
{
	protocol_commands.emplace(CommandName::GET_SYSTEM_INFO_1, std::make_unique<Get_System_Info_1>());
}

void Protocol_MasterSlave::runCommand(const Protocol_MasterSlave::CommandName cmd, const QList<QVariant> &input)
{
	if (!protocol_commands.count(cmd)) {
		QMetaEnum metaEnum = QMetaEnum::fromType<Protocol_MasterSlave::CommandName>();
		m_protocolOutput << "Unsupported command: " << metaEnum.valueToKey(static_cast<int>(cmd));
		Q_EMIT protocolOutputChanged();
		return;
	}

	QByteArray bytesToBeWriteen;
	bytesToBeWriteen.append(0x55);
	bytesToBeWriteen.append(static_cast<char>(cmd));

	protocol_commands.at(cmd)->masterCommand(input, bytesToBeWriteen);




}

}
