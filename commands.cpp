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

int Get_Tags_Number_And_Position_6To11::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(7, 0x0);

	return 0;
}

int Get_Tags_Number_And_Position_6To11::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != 7 || input.at(0) != 0x0)
		return -1;

	const auto p6 = static_cast<int>(input.at(1));
	if (p6 != pos6) {
		pos6 = p6;
		Q_EMIT pos6Changed();
	}
	const auto p7 = static_cast<int>(input.at(2));
	if (p7 != pos7) {
		pos7 = p7;
		Q_EMIT pos7Changed();
	}
	const auto p8 = static_cast<int>(input.at(3));
	if (p8 != pos8) {
		pos8 = p8;
		Q_EMIT pos8Changed();
	}
	const auto p9 = static_cast<int>(input.at(4));
	if (p9 != pos9) {
		pos9 = p9;
		Q_EMIT pos9Changed();
	}
	const auto p10 = static_cast<int>(input.at(5));
	if (p10 != pos10) {
		pos10 = p10;
		Q_EMIT pos10Changed();
	}
	const auto p11 = static_cast<int>(input.at(6));
	if (p11 != pos11) {
		pos11 = p11;
		Q_EMIT pos11Changed();
	}

	output = { pos6, pos7, pos8, pos9, pos10, pos11 };
	return 0;
}

int Get_Tags_Number_And_Position_12To17::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(7, 0x0);

	return 0;
}

int Get_Tags_Number_And_Position_12To17::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != 7 || input.at(0) != 0x0)
		return -1;

	const auto p12 = static_cast<int>(input.at(1));
	if (p12 != pos12) {
		pos12 = p12;
		Q_EMIT pos12Changed();
	}
	const auto p13 = static_cast<int>(input.at(2));
	if (p13 != pos13) {
		pos13 = p13;
		Q_EMIT pos13Changed();
	}
	const auto p14 = static_cast<int>(input.at(3));
	if (p14 != pos14) {
		pos14 = p14;
		Q_EMIT pos14Changed();
	}
	const auto p15 = static_cast<int>(input.at(4));
	if (p15 != pos15) {
		pos15 = p15;
		Q_EMIT pos15Changed();
	}
	const auto p16 = static_cast<int>(input.at(5));
	if (p16 != pos16) {
		pos16 = p16;
		Q_EMIT pos16Changed();
	}
	const auto p17 = static_cast<int>(input.at(6));
	if (p17 != pos17) {
		pos17 = p17;
		Q_EMIT pos17Changed();
	}

	output = { pos12, pos13, pos14, pos15, pos16, pos17 };
	return 0;
}

int Get_Tags_Number_And_Position_18To23::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(7, 0x0);

	return 0;
}

int Get_Tags_Number_And_Position_18To23::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != 7 || input.at(0) != 0x0)
		return -1;

	const auto p18 = static_cast<int>(input.at(1));
	if (p18 != pos18) {
		pos18 = p18;
		Q_EMIT pos18Changed();
	}
	const auto p19 = static_cast<int>(input.at(2));
	if (p19 != pos19) {
		pos19 = p19;
		Q_EMIT pos19Changed();
	}
	const auto p20 = static_cast<int>(input.at(3));
	if (p20 != pos20) {
		pos20 = p20;
		Q_EMIT pos20Changed();
	}
	const auto p21 = static_cast<int>(input.at(4));
	if (p21 != pos21) {
		pos21 = p21;
		Q_EMIT pos21Changed();
	}
	const auto p22 = static_cast<int>(input.at(5));
	if (p22 != pos22) {
		pos22 = p22;
		Q_EMIT pos22Changed();
	}
	const auto p23 = static_cast<int>(input.at(6));
	if (p23 != pos23) {
		pos23 = p23;
		Q_EMIT pos23Changed();
	}

	output = { pos18, pos19, pos20, pos21, pos22, pos23 };
	return 0;
}




}
