#ifndef COMMANDS_H
#define COMMANDS_H

#include <QVariantList>
#include <QByteArray>
#include <QObject>
#include <QTime>

#include <qqml.h>

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


}

#endif // COMMANDS_H
