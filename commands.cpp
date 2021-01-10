#include "commands.hpp"

namespace bioconverter {

int Get_System_Info_1::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(7, 0x0);

	return 0;
}

int Get_System_Info_1::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != 7)
		return -1;

	if (static_cast<unsigned int>(input.at(0)) != status) {
		status = input.at(0);
		Q_EMIT statusChanged();
	}
	if (static_cast<unsigned int>(input.at(1)) != action) {
		action = input.at(0);
		Q_EMIT actionChanged();
	}
	if (static_cast<unsigned int>(input.at(2)) != foodAvailable) {
		foodAvailable = input.at(0);
		Q_EMIT foodAvailableChanged();
	}
	if (static_cast<unsigned int>(input.at(3)) != positionToProcess) {
		positionToProcess = input.at(0);
		Q_EMIT positionToProcessChanged();
	}
	if (static_cast<unsigned int>(input.at(3) << 8 | input.at(4)) != functionInProgress) {
		functionInProgress = input.at(0);
		Q_EMIT functionInProgressChanged();
	}
	if (static_cast<unsigned int>(input.at(0)) != errorOccured) {
		errorOccured = input.at(0);
		Q_EMIT errorOccuredChanged();
	}

	output = { status, action, foodAvailable, positionToProcess, functionInProgress, errorOccured };
	return 0;
}




}
