#ifndef COMMANDS_H
#define COMMANDS_H

#include <QVariantList>
#include <QByteArray>
#include <QObject>
#include <QTime>

#include <qqml.h>

#include <QQmlListProperty>

namespace bioconverter {

class Command
{
public:
	Command() = default;
	virtual ~Command() = default;

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const = 0;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) = 0;
};

class Get_System_Info_1 : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Get_System_Info_1)
	QML_UNCREATABLE("Get_System_Info_1 is always a named property of protocol")

	Q_PROPERTY(Status status READ getStatus NOTIFY statusChanged)
	Q_PROPERTY(Action action READ getAction NOTIFY actionChanged)
	Q_PROPERTY(FoodAvailable foodAvailable READ getFoodAvailable NOTIFY foodAvailableChanged)
	Q_PROPERTY(int positionToProcess READ getPositionToProcess NOTIFY positionToProcessChanged)
	Q_PROPERTY(Function functionInProgress READ getFunctionInProgress NOTIFY functionInProgressChanged)
	Q_PROPERTY(Error errorOccured READ getErrorOccured NOTIFY errorOccuredChanged)

public:
	explicit Get_System_Info_1(QObject *parent = nullptr) :
		QObject(parent),
		status{},
		action{},
		foodAvailable{},
		positionToProcess{},
		functionInProgress{},
		errorOccured{}
	{};

	enum class Status {
		SYS_STS_NOT_INITIALIZED			   	= 0,

		SYS_STS_AUTO_TASK_RUNNING_ERROR   	= 1,
		//...
		SYS_STS_AUTO_TASK_RUNNING     		= 10,
		SYS_STS_AUTO_TASK_NOT_RUNNING 		= 11,
		SYS_STS_USER_EVENT_MANAGE     		= 12,
		//...
		SYS_STS_AUTO_MANAGE_ERROR           = 20
	};
	Q_ENUM(Status);

	enum class Action {
		SYS_ACTION_NO_ACTION					= 0,
		SYS_ACTION_USER_INSERT_NEW_CONTAINER 	= 1,
		SYS_ACTION_USER_SHOW_CONTAINER          = 2
	};
	Q_ENUM(Action);

	enum class FoodAvailable {
		FOOD_NOT_INITIALIZED    = 0,
		FOOD_AVAILABLE			= 1,
		FOOD_NOT_AVAILABLE		= 2,
		FOOD_IS_ABOUT_TO_END	= 3,
	};
	Q_ENUM(FoodAvailable);

	enum class Function {

		FUNCTION_NO_RUN										= 0x0000,

		FUNCTION_REFILL_CONTAINER   						= 0x1000, //functionInProgress_H  High bits
		FUNCTION_MOVE_CONTAINER_FROM_FLOOR_TO_EXIT          = 0x2000, //functionInProgress_H  High bits
		FUNCTION_SYS_ACTION_USER_INSERT_NEW_CONTAINER       = 0x3000, //functionInProgress_H  High bits
		FUNCTION_SYS_ACTION_USER_SHOW_CONTAINER             = 0x4000, //functionInProgress_H  High bits
		FUNCTION_SUPERVISOR_MANAGE_ERROR                    = 0x5000, //functionInProgress_H  High bits

		FUNCTION_MOVE_CONTAINER_FROM_FOOD_PLATFORM_TO_FLOOR = 0x0100, //functionInProgress_H  Low bits
		FUNCTION_MOVE_CONTAINER_FROM_FLOOR_TO_FOOD_PALTFORM = 0x0200, //functionInProgress_H  Low bits
		FUNCTION_FOOD_IMMITION 								= 0x0300, //functionInProgress_H  Low bits

		FUNCTION_EXTRACT_CONTAINER_FROM_FLOOR               = 0x10, //functionInProgress_L  High bits
		FUNCTION_INSERT_CONTAINER_INTO_FLOOR                = 0x20, //functionInProgress_L  High bits
		FUNCTION_AZIONA_DOSATORE                            = 0x30, //functionInProgress_L  High bits

		FUNCTION_SET_POSITION_NASTRINO_ASCENSORE            = 0x01, //functionInProgress_L  Low bits
	};
	Q_ENUM(Function);

	enum class Error {
	  NO_ERROR          = 0,
	  ERROR_GENERIC,
	  HOME_ERROR_Z,
	  HOME_NO_ERROR_Z,
	  CHECKSUM_ERR,
	  INDEX_NOT_VALID,

	  //...
	  ERR_ATT_FOOD_NOT_AVAILABLE			= 50,
	  ERR_ATT_AUTO_TASK_RUNNING				= 51,
	  ERR_ATT_NO_CONTAINER_ON_FOOD_PLATFORM = 52,
	  ERR_ATT_INDEX_OVERRUN                 = 53,
	  ERR_ATT_CONTAINER_IS_IN_PROCESS       = 54,
	  ERR_ATT_FALSE_EVENT_FROM_TERMINAL     = 55,
	};
	Q_ENUM(Error);

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

private:
	Status getStatus() { return status; };
	Action getAction() { return action; };
	FoodAvailable getFoodAvailable() { return foodAvailable; };
	unsigned int getPositionToProcess() { return positionToProcess; };
	Function getFunctionInProgress() { return functionInProgress; };
	Error getErrorOccured() { return errorOccured; };

Q_SIGNALS:
	void statusChanged();
	void actionChanged();
	void foodAvailableChanged();
	void positionToProcessChanged();
	void functionInProgressChanged();
	void errorOccuredChanged();

private:
	Status status;
	Action action;
	FoodAvailable foodAvailable;
	int positionToProcess;
	Function functionInProgress;
	Error errorOccured;
};


class Get_System_Info_2 : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Get_System_Info_2)
	QML_UNCREATABLE("Get_System_Info_2 is always a named property of protocol")

	Q_PROPERTY(QTime swapCycleTime READ getSwapCycleTime NOTIFY swapCycleTimeChanged)

public:
	explicit Get_System_Info_2(QObject *parent = nullptr) :
		QObject(parent),
		swapCycleTime{}
	{};

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

private:
	QTime getSwapCycleTime() { return swapCycleTime; };

Q_SIGNALS:
	void swapCycleTimeChanged();

private:
	QTime swapCycleTime;
};

class Get_Tags_Number_And_Position_0To5 : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Get_Tags_Number_And_Position_0To5)
	QML_UNCREATABLE("Get_Tags_Number_And_Position_0To5 is always a named property of protocol")

	Q_PROPERTY(int pos0 READ getPos0 NOTIFY pos0Changed)
	Q_PROPERTY(int pos1 READ getPos1 NOTIFY pos1Changed)
	Q_PROPERTY(int pos2 READ getPos2 NOTIFY pos2Changed)
	Q_PROPERTY(int pos3 READ getPos3 NOTIFY pos3Changed)
	Q_PROPERTY(int pos4 READ getPos4 NOTIFY pos4Changed)
	Q_PROPERTY(int pos5 READ getPos5 NOTIFY pos5Changed)

public:
	explicit Get_Tags_Number_And_Position_0To5(QObject *parent = nullptr) :
		QObject(parent),
		pos0{-1},
		pos1{-1},
		pos2{-1},
		pos3{-1},
		pos4{-1},
		pos5{-1}
	{};

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

private:
	int getPos0() { return pos0; };
	int getPos1() { return pos1; };
	int getPos2() { return pos2; };
	int getPos3() { return pos3; };
	int getPos4() { return pos4; };
	int getPos5() { return pos5; };

Q_SIGNALS:
	void pos0Changed();
	void pos1Changed();
	void pos2Changed();
	void pos3Changed();
	void pos4Changed();
	void pos5Changed();

private:
	int pos0;
	int pos1;
	int pos2;
	int pos3;
	int pos4;
	int pos5;
};

class Get_Tags_Number_And_Position_6To11 : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Get_Tags_Number_And_Position_6To11)
	QML_UNCREATABLE("Get_Tags_Number_And_Position_6To11 is always a named property of protocol")

	Q_PROPERTY(int pos6 READ getPos6   NOTIFY pos6Changed)
	Q_PROPERTY(int pos7 READ getPos7   NOTIFY pos7Changed)
	Q_PROPERTY(int pos8 READ getPos8   NOTIFY pos8Changed)
	Q_PROPERTY(int pos9 READ getPos9   NOTIFY pos9Changed)
	Q_PROPERTY(int pos10 READ getPos10 NOTIFY pos10Changed)
	Q_PROPERTY(int pos11 READ getPos11 NOTIFY pos11Changed)

public:
	explicit Get_Tags_Number_And_Position_6To11(QObject *parent = nullptr) :
		QObject(parent),
		pos6 {-1},
		pos7 {-1},
		pos8 {-1},
		pos9 {-1},
		pos10{-1},
		pos11{-1}
	{};

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

private:
	int getPos6 () { return pos6 ; };
	int getPos7 () { return pos7 ; };
	int getPos8 () { return pos8 ; };
	int getPos9 () { return pos9 ; };
	int getPos10() { return pos10; };
	int getPos11() { return pos11; };

Q_SIGNALS:
	void pos6Changed();
	void pos7Changed();
	void pos8Changed();
	void pos9Changed();
	void pos10Changed();
	void pos11Changed();

private:
	int pos6 ;
	int pos7 ;
	int pos8 ;
	int pos9 ;
	int pos10;
	int pos11;
};

class Get_Tags_Number_And_Position_12To17 : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Get_Tags_Number_And_Position_12To17)
	QML_UNCREATABLE("Get_Tags_Number_And_Position_12To17 is always a named property of protocol")

	Q_PROPERTY(int pos12 READ getPos12 NOTIFY pos12Changed)
	Q_PROPERTY(int pos13 READ getPos13 NOTIFY pos13Changed)
	Q_PROPERTY(int pos14 READ getPos14 NOTIFY pos14Changed)
	Q_PROPERTY(int pos15 READ getPos15 NOTIFY pos15Changed)
	Q_PROPERTY(int pos16 READ getPos16 NOTIFY pos16Changed)
	Q_PROPERTY(int pos17 READ getPos17 NOTIFY pos17Changed)

public:
	explicit Get_Tags_Number_And_Position_12To17(QObject *parent = nullptr) :
		QObject(parent),
		pos12{-1},
		pos13{-1},
		pos14{-1},
		pos15{-1},
		pos16{-1},
		pos17{-1}
	{};

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

private:
	int getPos12() { return pos12; };
	int getPos13() { return pos13; };
	int getPos14() { return pos14; };
	int getPos15() { return pos15; };
	int getPos16() { return pos16; };
	int getPos17() { return pos17; };

Q_SIGNALS:
	void pos12Changed();
	void pos13Changed();
	void pos14Changed();
	void pos15Changed();
	void pos16Changed();
	void pos17Changed();

private:
	int pos12;
	int pos13;
	int pos14;
	int pos15;
	int pos16;
	int pos17;
};

class Get_Tags_Number_And_Position_18To23 : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Get_Tags_Number_And_Position_18To23)
	QML_UNCREATABLE("Get_Tags_Number_And_Position_18To23 is always a named property of protocol")

	Q_PROPERTY(int pos18 READ getPos18 NOTIFY pos18Changed)
	Q_PROPERTY(int pos19 READ getPos19 NOTIFY pos19Changed)
	Q_PROPERTY(int pos20 READ getPos20 NOTIFY pos20Changed)
	Q_PROPERTY(int pos21 READ getPos21 NOTIFY pos21Changed)
	Q_PROPERTY(int pos22 READ getPos22 NOTIFY pos22Changed)
	Q_PROPERTY(int pos23 READ getPos23 NOTIFY pos23Changed)

public:
	explicit Get_Tags_Number_And_Position_18To23(QObject *parent = nullptr) :
		QObject(parent),
		pos18{-1},
		pos19{-1},
		pos20{-1},
		pos21{-1},
		pos22{-1},
		pos23{-1}
	{};

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

private:
	int getPos18() { return pos18; };
	int getPos19() { return pos19; };
	int getPos20() { return pos20; };
	int getPos21() { return pos21; };
	int getPos22() { return pos22; };
	int getPos23() { return pos23; };

Q_SIGNALS:
	void pos18Changed();
	void pos19Changed();
	void pos20Changed();
	void pos21Changed();
	void pos22Changed();
	void pos23Changed();

private:
	int pos18;
	int pos19;
	int pos20;
	int pos21;
	int pos22;
	int pos23;
};

class Container_Parameters1 : public QObject
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Container_Parameters1)
	QML_UNCREATABLE("Container_Parameters1 is always a named property of Get_Signle_Container_Parameters1_By_Pos")

//TODO: use enums instead
	Q_PROPERTY(int status READ getStatus NOTIFY statusChanged)
	Q_PROPERTY(int foodtype READ getFoodtype NOTIFY foodtypeChanged)
	Q_PROPERTY(int foodquantity READ getFoodquantity NOTIFY foodquantityChanged)
	Q_PROPERTY(QTime foodcycletime READ getFoodcycletime NOTIFY foodcycletimeChanged)
	Q_PROPERTY(int foodnumberofcycles READ getFoodnumberofcycles NOTIFY foodnumberofcyclesChanged)

public:
	explicit Container_Parameters1(QObject *parent = nullptr) :
		QObject(parent),
		status{-1},
		foodtype{-1},
		foodquantity{-1},
		foodcycletime(),
		foodnumberofcycles{-1}
	{}

	void setStatus(int newstatus) {
		if (newstatus != status) {
			status = newstatus;
			Q_EMIT statusChanged();
		}
	}
	void setFoodtype(int newfoodtype) {
		if (newfoodtype != foodtype) {
			foodtype = newfoodtype;
			Q_EMIT foodtypeChanged();
		}
	}
	void setFoodquantity(int newfoodquantity) {
		if (newfoodquantity != foodquantity) {
			foodquantity = newfoodquantity;
			Q_EMIT foodquantityChanged();
		}
	}
	void setFoodcycletime(QTime newfoodcycletime) {
		if (newfoodcycletime != foodcycletime) {
			foodcycletime = newfoodcycletime;
			Q_EMIT foodcycletimeChanged();
		}
	}
	void setFoodnumberofcycles(int newfoodnumberofcycles) {
		if (newfoodnumberofcycles != foodnumberofcycles) {
			foodnumberofcycles = newfoodnumberofcycles;
			Q_EMIT foodnumberofcyclesChanged();
		}
	}

Q_SIGNALS:
	void statusChanged();
	void foodtypeChanged();
	void foodquantityChanged();
	void foodcycletimeChanged();
	void foodnumberofcyclesChanged();

private:
	int getStatus() { return status; };
	int getFoodtype() { return foodtype; };
	int getFoodquantity() { return foodquantity; };
	QTime getFoodcycletime() { return foodcycletime; };
	int getFoodnumberofcycles() { return foodnumberofcycles; };

	int status;
	int foodtype;
	int foodquantity;
	QTime foodcycletime;
	int foodnumberofcycles;
};

constexpr int NUMBER_OF_POSITIONS { 24 };

class Get_Single_Container_Parameters1_By_Pos : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Get_Signle_Container_Parameters1_By_Pos)
	QML_UNCREATABLE("Get_Signle_Container_Parameters1_By_Pos is always a named property of protocol")

	Q_PROPERTY(QQmlListProperty<Container_Parameters1> containers READ getContainers)

public:
	explicit Get_Single_Container_Parameters1_By_Pos(QObject *parent = nullptr) :
		QObject(parent)
	{
		for (unsigned int i = 0; i < NUMBER_OF_POSITIONS; i++) {
			containers.push_back(new Container_Parameters1(this));
		}
	};

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

private:
	QQmlListProperty<Container_Parameters1> getContainers() {
		return QQmlListProperty<Container_Parameters1>(this, &containers);
	}

	QList<Container_Parameters1 *> containers;
};

}

#endif // COMMANDS_H
