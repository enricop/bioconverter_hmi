#ifndef SERIALPORT_READERWRITER_H
#define SERIALPORT_READERWRITER_H

#include <QObject>
#include <QSerialPort>

#include <QByteArray>
#include <QTextStream>

#include <memory>

class QTimer;

namespace bioconverter {

class SerialPort_ReaderWriter : public QObject
{
	Q_OBJECT

	Q_DISABLE_COPY(SerialPort_ReaderWriter)

	Q_PROPERTY(QString readOutput MEMBER readOutput NOTIFY readOutputChanged)
	Q_PROPERTY(QString writeOutput MEMBER writeOutput NOTIFY writeOutputChanged)
	Q_PROPERTY(QString controlOutput MEMBER controlOutput NOTIFY controlOutputChanged)

public:
	explicit SerialPort_ReaderWriter(QObject *parent = nullptr);
	~SerialPort_ReaderWriter();

	QStringList getAvailableSerialPorts();

	int init(const QString serialportname);

public slots:
	qint64 write(const QByteArray &writeData);

private slots:
	void handleReadyRead();
	void handleReadTimeout();

	void handleBytesWritten(qint64 bytes);
	void handleWriteTimeout();

	void handleError(QSerialPort::SerialPortError error);

signals:
	void readOutputChanged();
	void writeOutputChanged();
	void controlOutputChanged();

private:
	std::unique_ptr<QSerialPort> serialPort;

	QByteArray m_readData;
	QByteArray m_writeData;

	QString readOutput;
	QString writeOutput;
	QString controlOutput;

	QTextStream m_readOutput;
	QTextStream m_writeOutput;
	QTextStream m_controlOutput;

	qint64 m_bytesWritten = 0;

	std::unique_ptr<QTimer> readTimer;
	std::unique_ptr<QTimer> writeTimer;

};

}

#endif // SERIALPORT_READERWRITER_H
