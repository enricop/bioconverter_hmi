#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>

#include <qqml.h>

#include <memory>

class QThread;

namespace bioconverter {

class SerialPort_ReaderWriter;

class Backend : public QObject
{
	Q_OBJECT

	Q_DISABLE_COPY(Backend)

	QML_NAMED_ELEMENT(Backend)

	Q_PROPERTY(SerialPort_ReaderWriter* serialport READ spgetter)

public:
	explicit Backend(QObject *parent = nullptr);

	~Backend();

	SerialPort_ReaderWriter* spgetter() const;

signals:

private:
	std::unique_ptr<QThread> serial_worker;
	std::unique_ptr<SerialPort_ReaderWriter> serial_instance;
};

}

#endif // BACKEND_H
