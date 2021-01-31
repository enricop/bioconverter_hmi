#ifndef PROTOCOL_ERROR_HPP
#define PROTOCOL_ERROR_HPP

#include <QObject>
#include <qqml.h>

namespace bioconverter {

Q_NAMESPACE

QML_NAMED_ELEMENT(Bioconverter)
QML_UNCREATABLE("ProtocolError: only enums")

enum class SlaveError {
	NO_ERROR          = 0,
	ERROR_GENERIC,
	HOME_ERROR_Z,
	HOME_NO_ERROR_Z,
	CHECKSUM_ERR,
	INDEX_NOT_VALID,
	//...
	ERR_ATT_FOOD_NOT_AVAILABLE			  = 50,
	ERR_ATT_AUTO_TASK_RUNNING			  = 51,
	ERR_ATT_NO_CONTAINER_ON_FOOD_PLATFORM = 52,
	ERR_ATT_INDEX_OVERRUN                 = 53,
	ERR_ATT_CONTAINER_IS_IN_PROCESS       = 54,
	ERR_ATT_FALSE_EVENT_FROM_TERMINAL     = 55,
};
Q_ENUM_NS(SlaveError);

enum class MasterError {
	NO_ERROR          = 0,
	EXECUTING_OTHER_COMMAND,
	UNSUPPORTED_COMMAND,
	INVALID_INPUT_DATA,
	SERIAL_WRITE_FAILED,
	INVALID_CURRENT_COMMAND,
	INVALID_HEADER_RECEIVED,
	INVALID_CHECKSUM_RECEIVED,
	INVALID_DATA_RECEIVED,
	SERIAL_PORT_ERROR
};
Q_ENUM_NS(MasterError);

}

#endif // PROTOCOL_ERROR_HPP
