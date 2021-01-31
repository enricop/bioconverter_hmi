import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import bioconverter 1.0

ApplicationWindow {
    width: 1000
    height: 1000
    visible: true
    title: qsTr("BIOCONVERTER HMI")

    Backend {
        id: bio_backend

        Component.onCompleted: {
            var ret = serialport.openSerialPort();
            if (ret) {
                thestackview.enabled = true;
            }
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&ContainersSwapping")
            Action { text: qsTr("&Info") }
        }
        Menu {
            title: qsTr("&Advanced")
            Action { text: qsTr("&ManualMode"); onTriggered: thestackview.push("qrc:/manualmode.qml"); }
        }
        Menu {
            title: qsTr("&Help")
            Action { text: qsTr("&About") }
        }
    }

    header: Header {

    }

    StackView {
        id: thestackview
        anchors.fill: parent
        initialItem: "qrc:/mainview.qml"
        enabled: false
    }

    footer: Footer {

    }

}

