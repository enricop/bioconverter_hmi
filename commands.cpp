#include "commands.hpp"

namespace bioconverter {

constexpr std::uint8_t CMD_PAYLOAD_LEN { 7 };

int Get_System_Info_1::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Get_System_Info_1::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
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

	const auto fu_hi = static_cast<std::uint8_t>(input.at(4));
	function1InProgress = static_cast<Function1>(fu_hi & 0xF0);
	Q_EMIT function1InProgressChanged();
	function2InProgress = static_cast<Function2>(fu_hi & 0x0F);
	Q_EMIT function2InProgressChanged();

	const auto fu_low = static_cast<std::uint8_t>(input.at(5));
	function3InProgress = static_cast<Function3>(fu_low & 0xF0);
	Q_EMIT function3InProgressChanged();
	function4InProgress = static_cast<Function4>(fu_low & 0x0F);
	Q_EMIT function4InProgressChanged();

	const auto e = static_cast<SlaveError>(input.at(6));
	if (e != errorOccurred) {
		errorOccurred = e;
		Q_EMIT errorOccurredChanged();
	}

	output = { QVariant::fromValue(status),
			   QVariant::fromValue(action),
			   QVariant::fromValue(foodAvailable),
			   positionToProcess,
			   QVariant::fromValue(function1InProgress),
			   QVariant::fromValue(function2InProgress),
			   QVariant::fromValue(function3InProgress),
			   QVariant::fromValue(function4InProgress),
			   QVariant::fromValue(errorOccurred) };
	return 0;
}

int Get_System_Info_2::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Get_System_Info_2::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

    qDebug() << "Get_System_Info_2 reply first byte: 0x" << +input.at(0);

	const auto swaptime = static_cast<std::uint8_t>(input.at(0));
	swapCycleTime = QDateTime();
	if (swaptime != 0xFF) {
		swapCycleTime = QDateTime::fromSecsSinceEpoch(swaptime*60*60, Qt::UTC);
	}
	Q_EMIT swapCycleTimeChanged();

    qDebug() << "Get_System_Info_2 reply second byte: 0x" << static_cast<std::uint8_t>(input.at(1));
    qDebug() << "Get_System_Info_2 reply third byte: 0x" << static_cast<std::uint8_t>(input.at(2));

	const auto remainswaptime = static_cast<std::uint16_t>(static_cast<std::uint8_t>(input.at(1)) << 8 | static_cast<std::uint8_t>(input.at(2)));
	remainingSwapCycleTime = QDateTime();
	if (remainswaptime != 0x00FF) {
		remainingSwapCycleTime = QDateTime::fromSecsSinceEpoch(remainswaptime*60, Qt::UTC);
	}
	Q_EMIT remainingSwapCycleTimeChanged();

	output = { swapCycleTime, remainingSwapCycleTime };
	return 0;
}

int Get_Tags_Number_And_Position_0To5::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Get_Tags_Number_And_Position_0To5::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN || input.at(0) != 0x0)
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

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Get_Tags_Number_And_Position_6To11::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN || input.at(0) != 0x0)
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

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Get_Tags_Number_And_Position_12To17::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN || input.at(0) != 0x0)
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

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Get_Tags_Number_And_Position_18To23::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN || input.at(0) != 0x0)
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

int Get_Single_Container_Parameters1_By_Pos::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	if (input.size() != 1) {
		return -1;
	}

	bool ok = false;
	const auto pos = input.at(0).toInt(&ok);

	if (!ok || pos < 0 || pos >= NUMBER_OF_POSITIONS) {
		return -2;
	}

	output.append(pos);
	output.append(6, 0x0);

	return 0;
}

int Get_Single_Container_Parameters1_By_Pos::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

	const auto pos = static_cast<int>(input.at(0));
	if (pos < 0 || pos >= NUMBER_OF_POSITIONS) {
		return -2;
	}

	containers.at(pos)->setStatus(static_cast<Container_Parameters1::Status>(input.at(1)));
	containers.at(pos)->setFoodtype(static_cast<int>(input.at(2)));
	containers.at(pos)->setFoodquantity(static_cast<int>(input.at(3)));

	const auto cycletime = static_cast<std::uint8_t>(input.at(4));
	auto cyclehours = QDateTime();
	if (cycletime != 0xFF) {
		cyclehours = QDateTime::fromSecsSinceEpoch(cycletime*60*60, Qt::UTC);
	}

	containers.at(pos)->setFoodcycletime(cyclehours);
	containers.at(pos)->setFoodcycles(static_cast<int>(input.at(5)));

	qDebug() << "cycletime:" << static_cast<std::uint8_t>(input.at(4));

	output = {
		pos,
		QVariant::fromValue(static_cast<Container_Parameters1::Status>(input.at(1))),
		static_cast<int>(input.at(2)),
		static_cast<int>(input.at(3)),
		cyclehours,
		static_cast<int>(input.at(5))
	};

	return 0;
}

int Get_Single_Container_Parameters2_By_Pos::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	if (input.size() != 1) {
		return -1;
	}

	bool ok = false;
	const auto pos = input.at(0).toInt(&ok);

	if (!ok || pos < 0 || pos >= NUMBER_OF_POSITIONS) {
		return -2;
	}

	output.append(pos);
	output.append(6, 0x0);

	return 0;
}

int Get_Single_Container_Parameters2_By_Pos::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

	const auto pos = static_cast<int>(input.at(0));
	if (pos < 0 || pos >= NUMBER_OF_POSITIONS) {
		return -2;
	}

	const auto rmntime = static_cast<std::uint16_t>(static_cast<std::uint8_t>(input.at(1)) << 8 | static_cast<std::uint8_t>(input.at(2)));
	auto rmnmins = QDateTime();
	if (rmntime != 0xFF) {
		rmnmins = QDateTime::fromSecsSinceEpoch(rmntime*60, Qt::UTC);
	}

	containers.at(pos)->setRemainingfoodcycletime(rmnmins);
	containers.at(pos)->setRemainingfoodcycles(static_cast<int>(input.at(3)));

	output = {
		pos,
		rmnmins,
		static_cast<int>(input.at(3))
	};

	return 0;
}

int Try_To_Insert_New_Container::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Try_To_Insert_New_Container::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

	for (const auto b : input) {
		if (b != 0x0)
			return -1;
	}

	output = {};
	return 0;
}

int Set_Single_Container_Parameters::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	if (input.size() != 5)
		return -1;

	output.append(1, 0x0);

	//da 0 a 10 food type
	//da 0 a 255 food quantity units
	//da 1 a 10 number of cycles
	//da 0 a 255 cycle time

	for (const auto &in : input) {
		bool ok = false;
		output.append(1, in.toInt(&ok));
		if (!ok)
			return -2;
	}

	output.append(1, 0x0);
	return 0;
}

int Set_Single_Container_Parameters::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

    const auto tag = static_cast<std::uint8_t>(input.at(0));
	if (tag < TAG_MIN || tag > TAG_MAX) {
		return -2;
	}

	newtag = tag;
	Q_EMIT newtagChanged();

	output = { newtag };
	return 0;
}

int Erase_EEPROM_Reset_System::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Erase_EEPROM_Reset_System::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

	for (const auto b : input) {
		if (b != 0x0)
			return -1;
	}

	output = {};
	return 0;
}

int Try_To_Show_Container::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	if (input.size() != 1)
		return -1;

	bool ok = false;
	const auto tag = input.at(0).toInt(&ok);
	if (tag < TAG_MIN || tag > TAG_MAX) {
		return -2;
	}
	output.append(1, tag);

	output.append(6, 0x0);
	return 0;
}

int Try_To_Show_Container::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

    const auto tag = static_cast<std::uint8_t>(input.at(0));
	if (tag < TAG_MIN || tag > TAG_MAX) {
		return -2;
	}

	output = { tag };
	return 0;
}

int Show_Container_Go_Back::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Show_Container_Go_Back::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

	output = {};
	return 0;
}

int Manage_Error::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Manage_Error::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

	output = {};
	return 0;
}

int Delete_All_Errors::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Delete_All_Errors::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

	output = {};
	return 0;
}

int Cancel_Container_By_Tag::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	if (input.size() != 1)
		return -1;

	bool ok = false;
	const auto tag = input.at(0).toInt(&ok);
	if (tag < TAG_MIN || tag > TAG_MAX) {
		return -2;
	}
	output.append(1, tag);

	output.append(6, 0x0);
	return 0;
}

int Cancel_Container_By_Tag::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

    const auto tag = static_cast<std::uint8_t>(input.at(0));
	/* slave does not return the TAG * /
	if (tag < TAG_MIN || tag > TAG_MAX) {
		return -2;
	}
	*/
	if (tag != 0)
		return -2;

	output = { tag };
	return 0;
}

int Abort_Manual_Action::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int Abort_Manual_Action::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

	output = {};
	return 0;
}

int Set_System_Parameters::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	if (input.size() != 1)
		return -1;

	bool ok;
	output.append(1, input.at(0).toUInt(&ok));

	if (!ok)
		return -2;

	output.append(6, 0x0);
	return 0;
}

int Set_System_Parameters::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

    qDebug() << "Set_System_Parameters reply first byte: 0x" << +input.at(0);

    const auto swap_hours = static_cast<std::uint8_t>(input.at(0));
	auto swap_time = QDateTime();
	if (swap_hours != 0xFF) {
		swap_time = QDateTime::fromSecsSinceEpoch(swap_hours*60*60, Qt::UTC);
	}

	newSwapCycleTime = swap_time;
	Q_EMIT newSwapCycleTimeChanged();

	output = { newSwapCycleTime };
	return 0;
}

int End_Rearing_Cycle::masterCommand(const QList<QVariant> &input, QByteArray &output) const
{
	Q_UNUSED(input);

	output.append(CMD_PAYLOAD_LEN, 0x0);

	return 0;
}

int End_Rearing_Cycle::slaveResponse(const QByteArray &input, QList<QVariant> &output)
{
	if (input.size() != CMD_PAYLOAD_LEN)
		return -1;

	output = {};
	return 0;
}

}
