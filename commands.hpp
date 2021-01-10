#ifndef COMMANDS_H
#define COMMANDS_H

#include <QVariantList>
#include <QByteArray>
#include <QObject>

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

class Get_System_Info_1 : public QObject,  public Command {

	Q_OBJECT

	QML_NAMED_ELEMENT(Get_System_Info_1)
	QML_UNCREATABLE("Get_System_Info_1 is always a named property of protocol")

	Q_PROPERTY(unsigned int status READ getStatus NOTIFY statusChanged)
	Q_PROPERTY(unsigned int action READ getAction NOTIFY actionChanged)
	Q_PROPERTY(unsigned int foodAvailable READ getFoodAvailable NOTIFY foodAvailableChanged)
	Q_PROPERTY(unsigned int positionToProcess READ getPositionToProcess NOTIFY positionToProcessChanged)
	Q_PROPERTY(unsigned int functionInProgress READ getFunctionInProgress NOTIFY functionInProgressChanged)
	Q_PROPERTY(unsigned int errorOccured READ getErrorOccured NOTIFY errorOccuredChanged)

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

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) override;

private:
	unsigned int getStatus() { return status; };
	unsigned int getAction() { return action; };
	unsigned int getFoodAvailable() { return foodAvailable; };
	unsigned int getPositionToProcess() { return positionToProcess; };
	unsigned int getFunctionInProgress() { return functionInProgress; };
	unsigned int getErrorOccured() { return errorOccured; };

Q_SIGNALS:
	void statusChanged();
	void actionChanged();
	void foodAvailableChanged();
	void positionToProcessChanged();
	void functionInProgressChanged();
	void errorOccuredChanged();

private:
	unsigned int status;
	unsigned int action;
	unsigned int foodAvailable;
	unsigned int positionToProcess;
	unsigned int functionInProgress;
	unsigned int errorOccured;
};

}

#endif // COMMANDS_H
