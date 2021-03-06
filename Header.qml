import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import bioconverter 1.0

ToolBar {
    id: theheader

    property int labelsize: 17

    property alias pollingenabled: getinfotimer.running

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.alignment: Qt.AlignVCenter

            ToolButton {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                text: "Status:\n" + bio_backend.protocol.systeminfo1.statusStr
                font.bold: true
                font.pixelSize: labelsize
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                text: "Current Action:\n" + bio_backend.protocol.systeminfo1.action
                font.bold: true
                font.pixelSize: labelsize
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                text: "Available Food:\n" + bio_backend.protocol.systeminfo1.foodAvailable
                font.bold: true
                font.pixelSize: labelsize
            }
        }
        RowLayout {

            ToolButton {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                text: "Position to process:\n" + bio_backend.protocol.systeminfo1.positionToProcess
                font.bold: true
                font.pixelSize: labelsize
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                text: "F1:\n" + bio_backend.protocol.systeminfo1.function1InProgress
                font.bold: true
                font.pixelSize: labelsize
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                text: "F2:\n" + bio_backend.protocol.systeminfo1.function2InProgress
                font.bold: true
                font.pixelSize: labelsize
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                text: "F3:\n" + bio_backend.protocol.systeminfo1.function3InProgress
                font.bold: true
                font.pixelSize: labelsize
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                text: "F4:\n" + bio_backend.protocol.systeminfo1.function4InProgress
                font.bold: true
                font.pixelSize: labelsize
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignVCenter
                Layout.fillWidth: true
                text: "Last Error Occurred:\n" + bio_backend.protocol.systeminfo1.errorOccurredStr
                font.bold: true
                font.pixelSize: labelsize
            }
        }
    }

    Timer {
        id: getinfotimer
        interval: 5000
        repeat: true
        running: false
        triggeredOnStart: false
        onTriggered: {
            bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_SYSTEM_INFO_1, []);
        }
    }

    Connections {
        enabled: theheader === visible && getinfotimer.running
        target: bio_backend.protocol
        function onCommandResult(cmd, master_error, proto_output, slave_error) {
            if (cmd == Protocol_MasterSlave.GET_SYSTEM_INFO_1) {
                if (master_error == Bioconverter.EXECUTING_OTHER_COMMAND) {
                    return;
                }

                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SYSTEM_ERROR)
                {
                    errordialog.title = "GET_SYSTEM_INFO_1 command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                    return;
                }
            }
        }
    }

    Connections {
        target: bio_backend.protocol.systeminfo1
        function onErrorOccurredChanged() {
            console.log("ErrorOccurred: ", bio_backend.protocol.systeminfo1.errorOccurred);
            if (bio_backend.protocol.systeminfo1.errorOccurred == Bioconverter.NO_SYSTEM_ERROR)
                return;

            thestackview.push("qrc:/erroroccurred.qml");
        }
    }

}
