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

int Get_System_Info_2::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(7, 0x0);

	return 0;
}

int Get_System_Info_2::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != 7)
		return -1;

	const auto hours = static_cast<int>(input.at(0));
	const auto minutes = static_cast<int>(input.at(1) << 8 | input.at(2));

	if (hours < 0 || minutes < 0) {
		swapCycleTime = QTime{};
		Q_EMIT swapCycleTimeChanged();
	} else {
		QTime t(hours, minutes);
		if (t != swapCycleTime) {
			swapCycleTime = t;
			Q_EMIT swapCycleTimeChanged();
		}
	}

	output = { swapCycleTime };
	return 0;
}

int Get_Tags_Number_And_Position_0To5::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(7, 0x0);

	return 0;
}

int Get_Tags_Number_And_Position_0To5::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != 7 || input.at(0) != 0x0)
		return -1;

	const auto p0 = static_cast<int>(input.at(1));
	if (p0 != pos0) {
		pos0 = p0;
		Q_EMIT pos0Changed();
	}
	const auto p1 = static_cast<int>(input.at(2));
	if (p1 != pos1) {
		pos1 = p1;
		Q_EMIT pos1Changed();
	}
	const auto p2 = static_cast<int>(input.at(3));
	if (p2 != pos2) {
		pos2 = p2;
		Q_EMIT pos2Changed();
	}
	const auto p3 = static_cast<int>(input.at(4));
	if (p3 != pos3) {
		pos3 = p3;
		Q_EMIT pos3Changed();
	}
	const auto p4 = static_cast<int>(input.at(5));
	if (p4 != pos4) {
		pos4 = p4;
		Q_EMIT pos4Changed();
	}
	const auto p5 = static_cast<int>(input.at(6));
	if (p5 != pos5) {
		pos5 = p5;
		Q_EMIT pos5Changed();
	}

	output = { pos0, pos1, pos2, pos3, pos4, pos5 };
	return 0;
}




}
