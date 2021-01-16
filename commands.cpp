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

	const auto s = static_cast<Status>(input.at(0));
	if (s != status) {
		status = s;
		Q_EMIT statusChanged();
	}
	const auto a = static_cast<Action>(input.at(1));
	if (a != action) {
		action = a;
		Q_EMIT actionChanged();
	}
	const auto f = static_cast<FoodAvailable>(input.at(2));
	if (f != foodAvailable) {
		foodAvailable = f;
		Q_EMIT foodAvailableChanged();
	}
	const auto p = static_cast<int>(input.at(3));
	if (p != positionToProcess) {
		positionToProcess = p;
		Q_EMIT positionToProcessChanged();
	}
	const auto fu = static_cast<Function>(input.at(4) << 8 | input.at(5));
	if (fu != functionInProgress) {
		functionInProgress = fu;
		Q_EMIT functionInProgressChanged();
	}
	const auto e = static_cast<Error>(input.at(6));
	if (e != errorOccured) {
		errorOccured = e;
		Q_EMIT errorOccuredChanged();
	}

	output = { QVariant::fromValue(status),
			   QVariant::fromValue(action),
			   QVariant::fromValue(foodAvailable),
			   positionToProcess,
			   QVariant::fromValue(functionInProgress),
			   QVariant::fromValue(errorOccured) };
	return 0;
}




}
