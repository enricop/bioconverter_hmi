import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import bioconverter 1.0

ApplicationWindow {
    width: 800
    height: 600
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
                    var ret = bio_backend.serialport.openSerialPort();
                    if (ret) {
                        listView.enabled = true;
                    }
                }
            }

            Label {
                text: bio_backend.serialport.controlOutput
            }
            Label {
                text: bio_backend.serialport.writeOutput
            }
            Label {
                text: bio_backend.serialport.readOutput
            }

        }

        RowLayout {

            Label {
                text: bio_backend.protocol.protocolOutput
            }

        }

        ListModel {
            id: commands

            ListElement {
                name: "GET_SYSTEM_INFO_1"
                cmd: Protocol_MasterSlave.GET_SYSTEM_INFO_1
                param: 0
            }
            ListElement {
                name: "GET_SYSTEM_INFO_2"
                cmd: Protocol_MasterSlave.GET_SYSTEM_INFO_2
                param: 0
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_0TO5"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_0TO5
                param: 0
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_6TO11"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_6TO11
                param: 0
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_12TO17"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_12TO17
                param: 0
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_18TO23"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_18TO23
                param: 0
            }
            ListElement {
                name: "GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS"
                cmd: Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS
                param: 2
            }
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: listView

                enabled: false

                width: parent.width
                model: commands
                delegate: ItemDelegate {
                    text: "Item " + name
                    width: listView.width
                    onClicked: {
                        cmd_timer.start();
                        bio_backend.protocol.runCommand(cmd, [param]);
                    }
                }
            }
        }

        Connections {
           target: bio_backend.protocol
           function onCommandResult(cmd, result, output) {
               cmd_timer.stop();
               console.log("Executed command: ", cmd);
               console.log("Result of command: ", result);
               console.log("Output of command: ", output);
           }
        }

        Timer {
            id: cmd_timer
            interval: 10000
            repeat: false
            running: false
            triggeredOnStart: false
            onTriggered: {
                console.log("Command timed out!");
            }
        }
    }
}

