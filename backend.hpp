#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

#include <qqml.h>

#include <memory>

namespace bioconverter {

class SerialPort_ReaderWriter;
class Protocol_MasterSlave;

class Backend : public QObject
{
	Q_OBJECT

	Q_DISABLE_COPY(Backend)

	QML_NAMED_ELEMENT(Backend)

	Q_PROPERTY(bioconverter::SerialPort_ReaderWriter* serialport READ spgetter CONSTANT)
	Q_PROPERTY(bioconverter::Protocol_MasterSlave* protocol READ protogetter CONSTANT)

public:
	explicit Backend(QObject *parent = nullptr);

	~Backend();

	SerialPort_ReaderWriter* spgetter() const;
	Protocol_MasterSlave* protogetter() const;

Q_SIGNALS:

private:
	std::shared_ptr<SerialPort_ReaderWriter> serial_instance;
	std::unique_ptr<Protocol_MasterSlave> protocol_instance;
};

}

#endif // BACKEND_H
