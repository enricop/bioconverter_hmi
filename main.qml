import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import bioconverter 1.0

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("Scroll")

    Backend {
        id: bio_backend
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {

            Button {
                text: "Init Serial Port"
                onClicked: {
                    bio_backend.serialport.openSerialPort();
                }
            }

            Label {
                text: bio_backend.serialport.controlOutput
            }

        }

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: listView

                enabled: false

                width: parent.width
                model: 20
                delegate: ItemDelegate {
                    text: "Item " + (index + 1)
                    width: listView.width
                }
            }
        }
    }
}

