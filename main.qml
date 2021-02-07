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
            title: qsTr("&Swapping")
            Action { text: qsTr("&Info"); onTriggered: {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_SYSTEM_INFO_2, []);
                    thestackview.push("qrc:/swappinginfo.qml");
                }
            }
        }
        Menu {
            title: qsTr("&Advanced")
            Action { text: qsTr("&Manual Mode"); onTriggered: thestackview.push("qrc:/manualmode.qml"); }
            Action { text: qsTr("&Manage Serial Port"); onTriggered: thestackview.push("qrc:/serialmng.qml"); }
        }
        Menu {
            title: qsTr("&Help")
            Action { text: qsTr("&About"); onTriggered: thestackview.push("qrc:/about.qml"); }
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

    Dialog {
        id: errordialog
        title: "\n"
        modal: true
        width: 500
        anchors.centerIn: parent
        standardButtons: Dialog.Ok
        contentItem: Label {
            id: errortext
            text: "\n"
        }
    }

    Dialog {
        id: successdialog
        title: "\n"
        modal: true
        width: 500
        anchors.centerIn: parent
        standardButtons: Dialog.Ok
        contentItem: Label {
            id: successtext
            text: "\n"
        }
    }

}

