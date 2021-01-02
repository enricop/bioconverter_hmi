#include "serialport_readerwriter.hpp"

#include <QSerialPortInfo>
#include <QTimer>
#include <QThread>
#include <QMetaEnum>

namespace bioconverter {

SerialPort_ReaderWriter::SerialPort_ReaderWriter(QObject *parent)
	: QObject(parent),
	  serialPort(std::make_unique<QSerialPort>()),
	  m_readOutput(&readOutput, QIODevice::WriteOnly),
	  m_writeOutput(&writeOutput, QIODevice::WriteOnly),
	  m_controlOutput(&controlOutput, QIODevice::WriteOnly)
{


}

SerialPort_ReaderWriter::~SerialPort_ReaderWriter()
{
	serialPort->close();
}

QStringList SerialPort_ReaderWriter::getAvailableSerialPorts()
{
	const auto serialPortInfos = QSerialPortInfo::availablePorts();

	m_controlOutput << "Total number of ports available: " << serialPortInfos.count() << "\n";

	const QString blankString = "N/A";

	QStringList serialports{};

	for (const auto &serialPortInfo : serialPortInfos) {
		QString description{};
		QString manufacturer{};
		QString serialNumber{};
		description = serialPortInfo.description();
		manufacturer = serialPortInfo.manufacturer();
		serialNumber = serialPortInfo.serialNumber();
		m_controlOutput << "\nPort: " << serialPortInfo.portName()
			<< "\nLocation: " << serialPortInfo.systemLocation()
			<< "\nDescription: " << (!description.isEmpty() ? description : blankString)
			<< "\nManufacturer: " << (!manufacturer.isEmpty() ? manufacturer : blankString)
			<< "\nSerial number: " << (!serialNumber.isEmpty() ? serialNumber : blankString)
			<< "\nVendor Identifier: " << (serialPortInfo.hasVendorIdentifier()
										 ? QByteArray::number(serialPortInfo.vendorIdentifier(), 16)
										 : blankString)
			<< "\nProduct Identifier: " << (serialPortInfo.hasProductIdentifier()
										  ? QByteArray::number(serialPortInfo.productIdentifier(), 16)
										  : blankString) << "\n";

		serialports.append(serialPortInfo.portName());
	}

	Q_EMIT controlOutputChanged();

	return serialports;
}

int SerialPort_ReaderWriter::init(const QString serialportname)
{
	serialPort->moveToThread(this->thread());
	writeTimer->moveToThread(this->thread());
	readTimer->moveToThread(this->thread());

	//READ
	QObject::connect(serialPort.get(), &QSerialPort::readyRead, this, &SerialPort_ReaderWriter::handleReadyRead, Qt::QueuedConnection);
	QObject::connect(readTimer.get(), &QTimer::timeout, this, &SerialPort_ReaderWriter::handleReadTimeout, Qt::QueuedConnection);

	//WRITE
	QObject::connect(serialPort.get(), &QSerialPort::bytesWritten, this, &SerialPort_ReaderWriter::handleBytesWritten, Qt::QueuedConnection);
	QObject::connect(writeTimer.get(), &QTimer::timeout, this, &SerialPort_ReaderWriter::handleWriteTimeout, Qt::QueuedConnection);

	//ERROR
	QObject::connect(serialPort.get(), &QSerialPort::errorOccurred, this, &SerialPort_ReaderWriter::handleError, Qt::QueuedConnection);

	this->thread()->start();

	m_controlOutput << "Opening Serial Port: " << serialportname;
	Q_EMIT controlOutputChanged();

	serialPort->setPortName(serialportname);

	serialPort->setBaudRate(QSerialPort::Baud115200);

	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setStopBits(QSerialPort::OneStop);

	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);

	return serialPort->open(QIODevice::ReadWrite);
}

qint64 SerialPort_ReaderWriter::write(const QByteArray &writeData)
{
	m_writeData = writeData;

	m_writeOutput << "Writing data: " << m_writeData << "\n";

	const qint64 bytesWritten = serialPort->write(writeData);

	if (bytesWritten == -1) {
		m_writeOutput << QObject::tr("Failed to write the data to port %1, error: %2")
							.arg(serialPort->portName())
							.arg(serialPort->errorString())
						 << "\n";
		Q_EMIT writeOutputChanged();
	} else if (bytesWritten != m_writeData.size()) {
		m_writeOutput << QObject::tr("Failed to write all the data to port %1, error: %2")
							.arg(serialPort->portName())
							.arg(serialPort->errorString())
						 << "\n";
		Q_EMIT writeOutputChanged();
	}

	writeTimer->start(5000);

	return bytesWritten;
}

void SerialPort_ReaderWriter::handleReadyRead()
{
	m_readData.append(serialPort->readAll());

	if (!readTimer->isActive())
		readTimer->start(5000);
}

void SerialPort_ReaderWriter::handleReadTimeout()
{
	if (m_readData.isEmpty()) {
		m_readOutput << QObject::tr("No data was currently available "
										"for reading from port %1")
							.arg(serialPort->portName())
						 << "\n";
	} else {
		m_readOutput << QObject::tr("Data successfully received from port %1")
							.arg(serialPort->portName())
						 << "\n";
		m_readOutput << m_readData << "\n";
	}
	Q_EMIT readOutputChanged();
}

void SerialPort_ReaderWriter::handleBytesWritten(qint64 bytes)
{
	m_bytesWritten += bytes;
	if (m_bytesWritten == m_writeData.size()) {
		m_bytesWritten = 0;
		m_writeOutput << QObject::tr("Data successfully sent to port %1")
							.arg(serialPort->portName()) << "\n";
		Q_EMIT writeOutputChanged();
	}
}

void SerialPort_ReaderWriter::handleWriteTimeout()
{
	m_writeOutput << QObject::tr("Operation timed out for port %1, error: %2")
						.arg(serialPort->portName())
						.arg(serialPort->errorString())
				  << "\n";
	Q_EMIT writeOutputChanged();
}

void SerialPort_ReaderWriter::handleError(QSerialPort::SerialPortError error)
{
	if (error == QSerialPort::WriteError) {
		m_writeOutput << QObject::tr("An I/O error occurred while writing"
										" the data to port %1, error: %2")
							.arg(serialPort->portName())
							.arg(serialPort->errorString())
						 << "\n";
		Q_EMIT writeOutputChanged();
	} else if (error == QSerialPort::ReadError) {
		m_readOutput << QObject::tr("An I/O error occurred while reading "
										"the data from port %1, error: %2")
							.arg(serialPort->portName())
							.arg(serialPort->errorString())
						 << "\n";
		Q_EMIT readOutputChanged();
	} else {
		QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::SerialPortError>();

		m_controlOutput << QObject::tr("An I/O error of type (%1) occurred"
										"from port %2, error: %3")
							.arg(metaEnum.valueToKey(error))
							.arg(serialPort->portName())
							.arg(serialPort->errorString())
						 << "\n";
		Q_EMIT controlOutputChanged();
	}
}

}
