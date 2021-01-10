#ifndef COMMANDS_H
#define COMMANDS_H

#include <QVariantList>
#include <QByteArray>

namespace bioconverter {

class Command
{
public:
	Command() = default;
	virtual ~Command() = default;

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const = 0;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) const = 0;
};

class Get_System_Info_1 : public QObject,  public Command {

	Q_OBJECT

	//Q_PROPERTY(int status MEMBER name WRITE setName NOTIFY nameChanged)
	//status	action	foodAvailable	positionToProcess	functionInProgress_H	functionInProgress_L	errorOccured

public:
	explicit Get_System_Info_1(QObject *parent = nullptr) : QObject(parent) {};

	virtual int masterCommand(const QList<QVariant> &input, QByteArray &output) const override;
	virtual int slaveResponse(const QByteArray &input, QList<QVariant> &output) const override;
};

}

#endif // COMMANDS_H
