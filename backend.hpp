#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

#include <qqml.h>

#include <memory>

namespace bioconverter {

class SerialPort_ReaderWriter;

class Backend : public QObject
{
	Q_OBJECT

	Q_DISABLE_COPY(Backend)

	QML_NAMED_ELEMENT(Backend)

	Q_PROPERTY(bioconverter::SerialPort_ReaderWriter* serialport READ spgetter CONSTANT)

public:
	explicit Backend(QObject *parent = nullptr);

	~Backend();

	SerialPort_ReaderWriter* spgetter() const;

Q_SIGNALS:

private:
	std::unique_ptr<SerialPort_ReaderWriter> serial_instance;
};

}

#endif // BACKEND_H
