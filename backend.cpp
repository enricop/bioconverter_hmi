#include "backend.hpp"
#include "serialport_readerwriter.hpp"
#include "protocol_masterslave.hpp"

#include <QThread>

namespace bioconverter {

Backend::Backend(QObject *parent)
	: QObject(parent),
	  protocol_instance(std::make_unique<Protocol_MasterSlave>()),
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

Protocol_MasterSlave *Backend::protogetter() const
{
	return protocol_instance.get();
}

}
