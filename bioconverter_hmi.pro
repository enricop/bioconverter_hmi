QT += quick core serialport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        backend.cpp \
        commands.cpp \
        main.cpp \
        protocol_masterslave.cpp \
        serialport_readerwriter.cpp

HEADERS += \
    backend.hpp \
    commands.hpp \
    protocol_error.hpp \
    protocol_masterslave.hpp \
	serialport_readerwriter.hpp \
    serialworker_bridge.hpp

RESOURCES += qml.qrc \
    assets.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

CONFIG += qmltypes

QML_IMPORT_NAME = bioconverter
QML_IMPORT_MAJOR_VERSION = 1
