#include "serialport_readerwriter.hpp"
#include "serialworker_bridge.hpp"

#include <QSerialPortInfo>
#include <QTimer>
#include <QThread>
#include <QMetaEnum>

namespace bioconverter {

SerialPort_ReaderWriter::SerialPort_ReaderWriter(QObject *parent)
	: QObject(parent),
	  serialWorker(std::make_unique<QThread>()),
	  serialPort(std::make_shared<QSerialPort>()),
	  serialBridge(std::make_unique<SerialWorker_Bridge>(serialPort)),
	  m_readData{},
	  m_writeData{},
	  m_bytesWritten{},
	  readOutput{},
	  writeOutput{},
	  controlOutput{},
	  m_readOutput(&readOutput),
	  m_writeOutput(&writeOutput),
	  m_controlOutput(&controlOutput),
	  readTimer(std::make_unique<QTimer>()),
	  writeTimer(std::make_unique<QTimer>())
{
	serialPort->moveToThread(serialWorker.get());
	writeTimer->moveToThread(serialWorker.get());
	readTimer->moveToThread(serialWorker.get());
	serialBridge->moveToThread(serialWorker.get());

	readTimer->setSingleShot(true);
	writeTimer->setSingleShot(true);

	//READ
	QObject::connect(serialPort.get(), &QSerialPort::readyRead, this, &SerialPort_ReaderWriter::handleReadyRead, Qt::QueuedConnection);
	QObject::connect(readTimer.get(), &QTimer::timeout, this, &SerialPort_ReaderWriter::handleReadTimeout, Qt::QueuedConnection);

	//WRITE
	QObject::connect(serialPort.get(), &QSerialPort::bytesWritten, this, &SerialPort_ReaderWriter::handleBytesWritten, Qt::QueuedConnection);
	QObject::connect(writeTimer.get(), &QTimer::timeout, this, &SerialPort_ReaderWriter::handleWriteTimeout, Qt::QueuedConnection);

	//ERROR
	QObject::connect(serialPort.get(), &QSerialPort::errorOccurred, this, &SerialPort_ReaderWriter::handleError, Qt::QueuedConnection);

	serialWorker->start();
}

SerialPort_ReaderWriter::~SerialPort_ReaderWriter()
{
	QMetaObject::invokeMethod(serialBridge.get(), "close", Qt::BlockingQueuedConnection);
	serialWorker->quit();
	serialWorker->wait();
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

bool SerialPort_ReaderWriter::openSerialPort()
{
	const auto serialportnames = getAvailableSerialPorts();
	if (serialportnames.empty()) {
		m_controlOutput << "No Serial Ports found in the system";
		Q_EMIT controlOutputChanged();
		return false;
	}

	const auto serialportname = serialportnames.first();

	m_controlOutput << "Opening Serial Port: " << serialportname;
	Q_EMIT controlOutputChanged();

	serialPort->setPortName(serialportname);

	serialPort->setBaudRate(QSerialPort::Baud115200);

	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setStopBits(QSerialPort::OneStop);

	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);

	bool result = false;
	bool ret = QMetaObject::invokeMethod(serialBridge.get(),
							  "open",
							  Qt::BlockingQueuedConnection,
							  Q_RETURN_ARG(bool, result));
	if (ret == false) {
		m_controlOutput << "Failed invoking 'open' method";
		return false;
	}
	if (result == false) {
		m_controlOutput << "Failed Opening Serial Port: " << serialportname << " with error " << serialPort->errorString();
	} else {
		m_controlOutput << "Serial Port: " << serialportname << " opened correctly!";
	}

	return result;
}

qint64 SerialPort_ReaderWriter::write(const QByteArray &writeData)
{
	m_writeData = writeData;

	m_writeOutput << "Writing data: " << m_writeData.toHex(':') << "\n";

	qint64 bytesWritten = -1;
	bool ret = QMetaObject::invokeMethod(serialBridge.get(),
							  "write",
							  Qt::BlockingQueuedConnection,
							  Q_RETURN_ARG(qint64, bytesWritten),
							  Q_ARG(const QByteArray &, m_writeData));
	if (ret == false) {
		m_controlOutput << "Failed invoking 'write' method";
		Q_EMIT writeOutputChanged();
		return bytesWritten;
	}

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
	} else {
		QMetaObject::invokeMethod(writeTimer.get(), "start", Qt::QueuedConnection, Q_ARG(int, 5000));
	}

	return bytesWritten;
}

void SerialPort_ReaderWriter::handleReadyRead()
{
	m_readData.append(serialPort->readAll());

	if (!readTimer->isActive()) {
		QMetaObject::invokeMethod(readTimer.get(), "start", Qt::QueuedConnection, Q_ARG(int, 5000));
	}
}

void SerialPort_ReaderWriter::handleReadTimeout()
{
	if (m_readData.isEmpty()) {
		m_readOutput << QObject::tr("No data was currently available "
										"for reading from port %1")
							.arg(serialPort->portName())
						 << "\n";
	} else {
		m_readOutput << QObject::tr("Data successfully received from port %1. Bytes: ")
							.arg(serialPort->portName())
						 << "\n";
		m_readOutput << m_readData.toHex(':') << "\n";

	}
	Q_EMIT dataRead(m_readData);
	m_readData.clear();
	Q_EMIT readOutputChanged();
}

void SerialPort_ReaderWriter::handleBytesWritten(qint64 bytes)
{
	m_bytesWritten += bytes;
	if (m_bytesWritten == m_writeData.size()) {
		QMetaObject::invokeMethod(writeTimer.get(), "stop", Qt::QueuedConnection);
		m_bytesWritten = 0;
		m_writeData.clear();
		m_writeOutput << QObject::tr("Data successfully written to port %1")
							.arg(serialPort->portName()) << "\n";
		Q_EMIT writeOutputChanged();
	}
}

void SerialPort_ReaderWriter::handleWriteTimeout()
{
	m_writeOutput << QObject::tr("Write operation timed out for port %1, error: %2")
						.arg(serialPort->portName())
						.arg(serialPort->errorString())
				  << "\n";
	m_bytesWritten = 0;
	m_writeData.clear();
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
	} else if (error != QSerialPort::SerialPortError::NoError) {
		QMetaEnum metaEnum = QMetaEnum::fromType<QSerialPort::SerialPortError>();

		m_controlOutput << QObject::tr("An I/O error of type (%1) occurred "
										"from port %2, error: %3")
							.arg(metaEnum.valueToKey(error))
							.arg(serialPort->portName())
							.arg(serialPort->errorString())
						 << "\n";
		Q_EMIT controlOutputChanged();
	}

	QMetaObject::invokeMethod(writeTimer.get(), "stop", Qt::QueuedConnection);
	QMetaObject::invokeMethod(readTimer.get(), "stop", Qt::QueuedConnection);
	m_bytesWritten = 0;
	m_writeData.clear();
	m_readData.clear();
}

}
