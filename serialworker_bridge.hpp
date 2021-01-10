#ifndef SERIALWORKER_BRIDGE_HPP
#define SERIALWORKER_BRIDGE_HPP

#include <QObject>
#include <QSerialPort>

#include <memory>

namespace bioconverter {

class SerialWorker_Bridge : public QObject
{
	Q_OBJECT

	Q_DISABLE_COPY(SerialWorker_Bridge)

public:
	explicit SerialWorker_Bridge(const std::shared_ptr<QSerialPort> sp,
									 QObject *parent = nullptr)
		: QObject(parent),
		  sp(sp)
	{}

	Q_INVOKABLE bool open() {
		return sp->open(QIODevice::ReadWrite);
	}
	Q_INVOKABLE qint64 write(const QByteArray &data) {
		return sp->write(data);
	}
	Q_INVOKABLE void close() {
		return sp->close();
	}

private:
	const std::shared_ptr<QSerialPort> sp;
};

}

#endif // SERIALWORKER_BRIDGE_HPP
