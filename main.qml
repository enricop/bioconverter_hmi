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
                theheader.pollingenabled = true;
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
            Action { text: qsTr("&Other Options"); onTriggered: thestackview.push("qrc:/otheroptions.qml"); }
        }
        Menu {
            title: qsTr("&Help")
            Action { text: qsTr("&About"); onTriggered: thestackview.push("qrc:/about.qml"); }
        }
    }

    header: Header {
        id: theheader
    }

    Image {
        id: mainbackground
        source: "qrc:/assets/000.png"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: parent.height / 2
    }

    StackView {
        id: thestackview
        anchors.fill: parent
        initialItem: "qrc:/mainview.qml"
        enabled: bio_backend.protocol.systeminfo1.status != Get_System_Info_1.SYS_STS_NOT_INITIALIZED
    }

    footer: Footer {
        id: thefooter
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

