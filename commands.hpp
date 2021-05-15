#ifndef COMMANDS_H
#define COMMANDS_H

#include <QVariantList>
#include <QByteArray>
#include <QObject>
#include <QDateTime>
#include <QMetaEnum>

#include <qqml.h>

#include <QQmlListProperty>

#include <protocol_error.hpp>

namespace bioconverter {

constexpr int NUMBER_OF_POSITIONS { 24 };
constexpr int TAG_MIN { 101 };
constexpr int TAG_MAX { TAG_MIN + NUMBER_OF_POSITIONS };

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
	Q_PROPERTY(QString statusStr READ getStatusStr NOTIFY statusChanged)
	Q_PROPERTY(QString action READ getAction NOTIFY actionChanged)
	Q_PROPERTY(QString foodAvailable READ getFoodAvailable NOTIFY foodAvailableChanged)
	Q_PROPERTY(int positionToProcess READ getPositionToProcess NOTIFY positionToProcessChanged)
	Q_PROPERTY(QString function1InProgress READ getFunction1InProgress NOTIFY function1InProgressChanged)
	Q_PROPERTY(QString function2InProgress READ getFunction2InProgress NOTIFY function2InProgressChanged)
	Q_PROPERTY(QString function3InProgress READ getFunction3InProgress NOTIFY function3InProgressChanged)
	Q_PROPERTY(QString function4InProgress READ getFunction4InProgress NOTIFY function4InProgressChanged)
	Q_PROPERTY(QString errorOccurredStr READ getErrorOccurredStr NOTIFY errorOccurredChanged)
	Q_PROPERTY(bioconverter::SlaveError errorOccurred READ getErrorOccurred NOTIFY errorOccurredChanged)

public:
	explicit Get_System_Info_1(QObject *parent = nullptr) :
		QObject(parent),
		status{},
		action{},
		foodAvailable{},
		positionToProcess{},
		function1InProgress{},
		function2InProgress{},
		function3InProgress{},
		function4InProgress{},
		errorOccurred{}
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
		ACTION_NO_ACTION					= 0,
		ACTION_USER_INSERT_NEW_CONTAINER 	= 1,
		ACTION_USER_SHOW_CONTAINER          = 2
	};
	Q_ENUM(Action);

	enum class FoodAvailable {
		FOOD_NOT_INITIALIZED    = 0,
		FOOD_AVAILABLE			= 1,
		FOOD_NOT_AVAILABLE		= 2,
		FOOD_IS_ABOUT_TO_END	= 3,
	};
	Q_ENUM(FoodAvailable);

	enum class Function1 {
		NO_RUN									   = 0x00,
		REFILL_CONTAINER						   = 0x10,
		MOVE_CONTAINER_FROM_FLOOR_TO_EXIT          = 0x20,
		SYS_ACTION_USER_INSERT_NEW_CONTAINER       = 0x30,
		SYS_ACTION_USER_SHOW_CONTAINER             = 0x40,
		SUPERVISOR_MANAGE_ERROR                    = 0x50,
	};
	Q_ENUM(Function1);
	enum class Function2 {
		NO_RUN									   = 0x00,
		MOVE_CONTAINER_FROM_FOOD_PLATFORM_TO_FLOOR = 0x01,
		MOVE_CONTAINER_FROM_FLOOR_TO_FOOD_PLATFORM = 0x02,
		FOOD_IMMITION 							   = 0x03,
	};
	Q_ENUM(Function2);
	enum class Function3 {
		NO_RUN						      = 0x00,
		EXTRACT_CONTAINER_FROM_FLOOR      = 0x10,
		INSERT_CONTAINER_INTO_FLOOR       = 0x20,
		AZIONA_DOSATORE                   = 0x30,
	};
	Q_ENUM(Function3);
	enum class Function4 {
		NO_RUN							  = 0x00,
		SET_POSITION_NASTRINO_ASCENSORE   = 0x01,
	};
	Q_ENUM(Function4);

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

private:
	Status getStatus() { return status; };
	QString getStatusStr() { return QMetaEnum::fromType<Status>().valueToKey(static_cast<int>(status)); };
	QString getAction() { return QMetaEnum::fromType<Action>().valueToKey(static_cast<int>(action)); };
	QString getFoodAvailable() { return QMetaEnum::fromType<FoodAvailable>().valueToKey(static_cast<int>(foodAvailable)); };
	unsigned int getPositionToProcess() { return positionToProcess; };
	QString getFunction1InProgress() { return QMetaEnum::fromType<Function1>().valueToKey(static_cast<int>(function1InProgress)); };
	QString getFunction2InProgress() { return QMetaEnum::fromType<Function2>().valueToKey(static_cast<int>(function2InProgress)); };
	QString getFunction3InProgress() { return QMetaEnum::fromType<Function3>().valueToKey(static_cast<int>(function3InProgress)); };
	QString getFunction4InProgress() { return QMetaEnum::fromType<Function4>().valueToKey(static_cast<int>(function4InProgress)); };
	QString getErrorOccurredStr() { return QMetaEnum::fromType<SlaveError>().valueToKey(static_cast<int>(errorOccurred)); };
	bioconverter::SlaveError getErrorOccurred() { return errorOccurred; };

Q_SIGNALS:
	void statusChanged();
	void actionChanged();
	void foodAvailableChanged();
	void positionToProcessChanged();
	void function1InProgressChanged();
	void function2InProgressChanged();
	void function3InProgressChanged();
	void function4InProgressChanged();
	void errorOccurredChanged();

private:
	Status status;
	Action action;
	FoodAvailable foodAvailable;
	int positionToProcess;
	Function1 function1InProgress;
	Function2 function2InProgress;
	Function3 function3InProgress;
	Function4 function4InProgress;
	SlaveError errorOccurred;
};


class Get_System_Info_2 : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Get_System_Info_2)
	QML_UNCREATABLE("Get_System_Info_2 is always a named property of protocol")

	Q_PROPERTY(QDateTime swapCycleTime READ getSwapCycleTime NOTIFY swapCycleTimeChanged)
	Q_PROPERTY(QDateTime remainingSwapCycleTime READ getRemainingSwapCycleTime NOTIFY remainingSwapCycleTimeChanged)

public:
	explicit Get_System_Info_2(QObject *parent = nullptr) :
		QObject(parent),
		swapCycleTime{},
		remainingSwapCycleTime{}
	{};

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

private:
	QDateTime getSwapCycleTime() { return swapCycleTime; };
	QDateTime getRemainingSwapCycleTime() { return remainingSwapCycleTime; };

Q_SIGNALS:
	void swapCycleTimeChanged();
	void remainingSwapCycleTimeChanged();

private:
	QDateTime swapCycleTime;
	QDateTime remainingSwapCycleTime;
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
	QML_UNCREATABLE("Container_Parameters1 is always a named property of Get_Single_Container_Parameters1_By_Pos")

	Q_PROPERTY(Status status READ getStatus NOTIFY statusChanged)
	Q_PROPERTY(int foodtype READ getFoodtype NOTIFY foodtypeChanged)
	Q_PROPERTY(int foodquantity READ getFoodquantity NOTIFY foodquantityChanged)
	Q_PROPERTY(QDateTime foodcycletime READ getFoodcycletime NOTIFY foodcycletimeChanged)
	Q_PROPERTY(int foodcycles READ getFoodcycles NOTIFY foodcyclesChanged)

public:
	explicit Container_Parameters1(QObject *parent = nullptr) :
		QObject(parent),
		status{Status::NOT_INSERTED},
		foodtype{-1},
		foodquantity{-1},
		foodcycletime(),
		foodcycles{-1}
	{}

	enum class Status {
		NOT_INSERTED = 0,
		INSERTED = 1,
		MOVING = 2
	};
	Q_ENUM(Status)

	void setStatus(Status newstatus) {
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
	void setFoodcycletime(QDateTime newfoodcycletime) {
		if (newfoodcycletime != foodcycletime) {
			foodcycletime = newfoodcycletime;
			Q_EMIT foodcycletimeChanged();
		}
	}
	void setFoodcycles(int newfoodnumberofcycles) {
		if (newfoodnumberofcycles != foodcycles) {
			foodcycles = newfoodnumberofcycles;
			Q_EMIT foodcyclesChanged();
		}
	}

Q_SIGNALS:
	void statusChanged();
	void foodtypeChanged();
	void foodquantityChanged();
	void foodcycletimeChanged();
	void foodcyclesChanged();

private:
	Status getStatus() { return status; };
	int getFoodtype() { return foodtype; };
	int getFoodquantity() { return foodquantity; };
	QDateTime getFoodcycletime() { return foodcycletime; };
	int getFoodcycles() { return foodcycles; };

	Status status;
	int foodtype;
	int foodquantity;
	QDateTime foodcycletime;
	int foodcycles;
};

class Get_Single_Container_Parameters1_By_Pos : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Get_Signle_Container_Parameters1_By_Pos)
	QML_UNCREATABLE("Get_Signle_Container_Parameters1_By_Pos is always a named property of protocol")

	Q_PROPERTY(QQmlListProperty<bioconverter::Container_Parameters1> containers READ getContainers)

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

class Container_Parameters2 : public QObject
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Container_Parameters2)
	QML_UNCREATABLE("Container_Parameters2 is always a named property of Get_Single_Container_Parameters2_By_Pos")

	Q_PROPERTY(QDateTime remainingfoodcycletime READ getRemainingfoodcycletime NOTIFY remainingfoodcycletimeChanged)
	Q_PROPERTY(int remainingfoodcycles READ getRemainingfoodcycles NOTIFY remainingfoodcyclesChanged)

public:
	explicit Container_Parameters2(QObject *parent = nullptr) :
		QObject(parent),
		remainingfoodcycletime(),
		remainingfoodcycles{-1}
	{}

	enum class Status {
		NOT_INSERTED = 0,
		INSERTED = 1,
		MOVING = 2
	};
	Q_ENUM(Status)

	void setRemainingfoodcycletime(QDateTime newremainingfoodcycletime) {
		if (newremainingfoodcycletime != remainingfoodcycletime) {
			remainingfoodcycletime = newremainingfoodcycletime;
			Q_EMIT remainingfoodcycletimeChanged();
		}
	}
	void setRemainingfoodcycles(int newremainingfoodcycles) {
		if (newremainingfoodcycles != remainingfoodcycles) {
			remainingfoodcycles = newremainingfoodcycles;
			Q_EMIT remainingfoodcyclesChanged();
		}
	}

Q_SIGNALS:
	void remainingfoodcycletimeChanged();
	void remainingfoodcyclesChanged();

private:
	QDateTime getRemainingfoodcycletime() { return remainingfoodcycletime; };
	int getRemainingfoodcycles() { return remainingfoodcycles; };

	QDateTime remainingfoodcycletime;
	int remainingfoodcycles;
};

class Get_Single_Container_Parameters2_By_Pos : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Get_Single_Container_Parameters2_By_Pos)
	QML_UNCREATABLE("Get_Single_Container_Parameters2_By_Pos is always a named property of protocol")

	Q_PROPERTY(QQmlListProperty<bioconverter::Container_Parameters2> containers READ getContainers)

public:
	explicit Get_Single_Container_Parameters2_By_Pos(QObject *parent = nullptr) :
		QObject(parent)
	{
		for (unsigned int i = 0; i < NUMBER_OF_POSITIONS; i++) {
			containers.push_back(new Container_Parameters2(this));
		}
	};

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

private:
	QQmlListProperty<Container_Parameters2> getContainers() {
		return QQmlListProperty<Container_Parameters2>(this, &containers);
	}

	QList<Container_Parameters2 *> containers;
};

class Try_To_Insert_New_Container : public Command
{
public:
	explicit Try_To_Insert_New_Container() { }

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;
};

class Set_Single_Container_Parameters : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Set_Single_Container_Parameters)
	QML_UNCREATABLE("Set_Single_Container_Parameters is always a named property of protocol")

	Q_PROPERTY(int newtag READ getNewtag NOTIFY newtagChanged)

public:
	explicit Set_Single_Container_Parameters(QObject *parent = nullptr) :
		QObject(parent),
		newtag{-1}
	{ }

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

Q_SIGNALS:
	void newtagChanged();

private:
	int getNewtag() { return newtag; }

	int newtag;
};

class Erase_EEPROM_Reset_System : public Command
{
public:
	explicit Erase_EEPROM_Reset_System() { }

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;
};

class Try_To_Show_Container : public Command
{
public:
	explicit Try_To_Show_Container() { }

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;
};

class Show_Container_Go_Back : public Command
{
public:
	explicit Show_Container_Go_Back() { }

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;
};

class Manage_Error : public Command
{
public:
	explicit Manage_Error() { }

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;
};

class Delete_All_Errors : public Command
{
public:
	explicit Delete_All_Errors() { }

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;
};

class Cancel_Container_By_Tag : public Command
{
public:
	explicit Cancel_Container_By_Tag() { }

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;
};

class Abort_Manual_Action : public Command
{
public:
	explicit Abort_Manual_Action() { }

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;
};

class Set_System_Parameters : public QObject,  public Command
{
	Q_OBJECT

	QML_NAMED_ELEMENT(Set_System_Parameters)
	QML_UNCREATABLE("Set_System_Parameters is always a named property of protocol")

	Q_PROPERTY(QDateTime newSwapCycleTime READ getNewSwapCycleTime NOTIFY newSwapCycleTimeChanged)

public:
	explicit Set_System_Parameters(QObject *parent = nullptr) :
		QObject(parent),
		newSwapCycleTime{}
	{ }

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

Q_SIGNALS:
	void newSwapCycleTimeChanged();

private:
	QDateTime getNewSwapCycleTime() { return newSwapCycleTime; }

	QDateTime newSwapCycleTime;
};

}

#endif // COMMANDS_H
