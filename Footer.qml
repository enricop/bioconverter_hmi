import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import bioconverter 1.0

Rectangle {
    border.color: "black"
    color: "lightgrey"
    height: 175
    width: parent.width

    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter

            Label {
                text: "Commands Output:"
            }
            Flickable {
                Layout.fillWidth: true
                Layout.fillHeight: true
                contentWidth: commandsoutput.width
                contentHeight: commandsoutput.height
                clip: true
                Label {
                    id: commandsoutput
                    text: "\n"
                }
                contentY: contentHeight - height
            }
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter

            Label {
                text: "Protocol Output:"
            }
            Flickable {
                Layout.fillWidth: true
                Layout.fillHeight: true
                contentWidth: protooutput.width
                contentHeight: protooutput.height
                clip: true
                Label {
                    id: protooutput
                    text: bio_backend.protocol.protocolOutput
                }
                contentY: contentHeight - height
            }
        }
    }

    Connections {
       target: bio_backend.protocol
       function onCommandResult(cmd, master_error, proto_output, slave_error) {
           commandsoutput.text = commandsoutput.text.concat("\nExecuted command: ", cmd);
           commandsoutput.text = commandsoutput.text.concat("\nmaster_error: ", master_error);
           commandsoutput.text = commandsoutput.text.concat("\nOutput of command: ", proto_output);
           commandsoutput.text = commandsoutput.text.concat("\nslave_error: ", slave_error);
           commandsoutput.text = commandsoutput.text.concat("\n -- ");
       }
    }


}
