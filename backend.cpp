#include "backend.hpp"
#include "serialport_readerwriter.hpp"

#include <QThread>

namespace bioconverter {

Backend::Backend(QObject *parent)
	: QObject(parent),
	  serial_worker(std::make_unique<QThread>()),
	  serial_instance(std::make_unique<SerialPort_ReaderWriter>())
{
	serial_instance->moveToThread(serial_worker.get());
}

Backend::~Backend()
{
	serial_worker->quit();
	serial_worker->wait();
}

SerialPort_ReaderWriter *Backend::spgetter() const
{
	return serial_instance.get();
}

}
