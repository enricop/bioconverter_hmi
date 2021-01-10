#ifndef SERIALPORT_READERWRITER_H
#define SERIALPORT_READERWRITER_H

#include <QObject>
#include <QSerialPort>

#include <QByteArray>
#include <QTextStream>

#include <qqml.h>

#include <memory>

class QTimer;
class QThread;

namespace bioconverter {

class SerialWorker_Bridge;

class SerialPort_ReaderWriter : public QObject
{
	Q_OBJECT

	Q_DISABLE_COPY(SerialPort_ReaderWriter)

	QML_NAMED_ELEMENT(SerialPort_ReaderWriter)
	QML_UNCREATABLE("SerialPort_ReaderWriter is always a named property of backend")

	Q_PROPERTY(QString readOutput READ getReadOutput NOTIFY readOutputChanged)
	Q_PROPERTY(QString writeOutput READ getWriteOutput NOTIFY writeOutputChanged)
	Q_PROPERTY(QString controlOutput READ getControlOutput NOTIFY controlOutputChanged)

public:
	explicit SerialPort_ReaderWriter(QObject *parent = nullptr);
	~SerialPort_ReaderWriter();

	QString getReadOutput() {
		return readOutput;
	}
	QString getWriteOutput() {
		return writeOutput;
	}
	QString getControlOutput() {
		return controlOutput;
	}

	qint64 write(const QByteArray &writeData);

	Q_INVOKABLE bool openSerialPort();

private Q_SLOTS:
	void handleReadyRead();
	void handleReadTimeout();

	void handleBytesWritten(qint64 bytes);
	void handleWriteTimeout();

	void handleError(QSerialPort::SerialPortError error);

Q_SIGNALS:
	void readOutputChanged();
	void writeOutputChanged();
	void controlOutputChanged();

	void dataRead(const QByteArray dataRead);

private:
	QStringList getAvailableSerialPorts();

	const std::unique_ptr<QThread> serialWorker;
	const std::shared_ptr<QSerialPort> serialPort;
	const std::unique_ptr<SerialWorker_Bridge> serialBridge;

	QByteArray m_readData;
	QByteArray m_writeData;
	qint64 m_bytesWritten;

	QString readOutput;
	QString writeOutput;
	QString controlOutput;

	QTextStream m_readOutput;
	QTextStream m_writeOutput;
	QTextStream m_controlOutput;

	std::unique_ptr<QTimer> readTimer;
	std::unique_ptr<QTimer> writeTimer;
};

}

#endif // SERIALPORT_READERWRITER_H
