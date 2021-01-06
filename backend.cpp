#include "backend.hpp"
#include "serialport_readerwriter.hpp"

#include <QThread>

namespace bioconverter {

Backend::Backend(QObject *parent)
	: QObject(parent),
	  serial_instance(std::make_unique<SerialPort_ReaderWriter>())
{

}

Backend::~Backend()
{

}

SerialPort_ReaderWriter *Backend::spgetter() const
{
	return serial_instance.get();
}

}
