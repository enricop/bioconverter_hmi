import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import bioconverter 1.0
import bioconverterenums 1.0

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

        Button {
            text: "Open Serial Port"
            onClicked: {
                var ret = bio_backend.serialport.openSerialPort();
                if (ret) {
                    listView.enabled = true;
                }
            }
        }

        Flickable {
            width: parent.width
            height: 150
            contentWidth: parent.width
            contentHeight: writeoutput.height

            Layout.fillWidth: true
            Layout.maximumHeight: 150
            Layout.minimumHeight: 150

            clip: true

            RowLayout {
                Label {
                    text: bio_backend.serialport.controlOutput
                }
                Label {
                    id: writeoutput
                    text: bio_backend.serialport.writeOutput
                }
                Label {
                    text: bio_backend.serialport.readOutput
                }
            }
            contentY: contentHeight - height
        }

        Flickable {
            width: parent.width
            height: 50
            contentWidth: parent.width
            contentHeight: protooutput.height

            Layout.fillWidth: true
            Layout.maximumHeight: 50
            Layout.minimumHeight: 50

            clip: true

            Label {
                id: protooutput
                text: bio_backend.protocol.protocolOutput
            }
            contentY: contentHeight - height
        }

        ListModel {
            id: commands

            ListElement {
                name: "GET_SYSTEM_INFO_1"
                cmd: Protocol_MasterSlave.GET_SYSTEM_INFO_1
                hasparams: false
            }
            ListElement {
                name: "GET_SYSTEM_INFO_2"
                cmd: Protocol_MasterSlave.GET_SYSTEM_INFO_2
                hasparams: false
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_0TO5"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_0TO5
                hasparams: false
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_6TO11"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_6TO11
                hasparams: false
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_12TO17"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_12TO17
                hasparams: false
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_18TO23"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_18TO23
                hasparams: false
            }
            ListElement {
                name: "GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS"
                cmd: Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS
                hasparams: true
            }
            ListElement {
                name: "GET_SINGLE_CONTAINER_PARAMETERS2_BY_POS"
                cmd: Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS2_BY_POS
                hasparams: true
            }
            ListElement {
                name: "TRY_TO_INSERT_NEW_CONTAINER"
                cmd: Protocol_MasterSlave.TRY_TO_INSERT_NEW_CONTAINER
                hasparams: false
            }
            ListElement {
                name: "SET_SINGLE_CONTAINER_PARAMETERS1"
                cmd: Protocol_MasterSlave.SET_SINGLE_CONTAINER_PARAMETERS1
                hasparams: true
            }
            ListElement {
                name: "ERASE_EEPROM_RESET_SYSTEM"
                cmd: Protocol_MasterSlave.ERASE_EEPROM_RESET_SYSTEM
                hasparams: false
            }
            ListElement {
                name: "TRY_TO_SHOW_CONTAINER"
                cmd: Protocol_MasterSlave.TRY_TO_SHOW_CONTAINER
                hasparams: true
            }
            ListElement {
                name: "SHOW_CONTAINER_GO_BACK"
                cmd: Protocol_MasterSlave.SHOW_CONTAINER_GO_BACK
                hasparams: false
            }
            ListElement {
                name: "MANAGE_ERROR"
                cmd: Protocol_MasterSlave.MANAGE_ERROR
                hasparams: false
            }
            ListElement {
                name: "DELETE_ALL_ERRORS"
                cmd: Protocol_MasterSlave.DELETE_ALL_ERRORS
                hasparams: false
            }
            ListElement {
                name: "CANCEL_CONTAINER_BY_TAG"
                cmd: Protocol_MasterSlave.CANCEL_CONTAINER_BY_TAG
                hasparams: true
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

                    property var param: []

                    contentItem: Rectangle {
                        RowLayout {
                            anchors.fill: parent
                            Button {
                                text: "Command: " + name
                                onClicked: {
                                    cmd_timer.start();
                                    bio_backend.protocol.runCommand(cmd, param);
                                }
                            }
                            TextField {
                                visible: hasparams
                                text: param.toString();
                                onEditingFinished: {
                                    param = text.split(',');
                                }
                            }
                        }
                    }
                }
            }
        }

        Flickable {
            id: commandscroll
            width: parent.width
            height: 150
            contentWidth: parent.width
            contentHeight: commandsoutput.height

            Layout.fillWidth: true
            Layout.maximumHeight: 150
            Layout.minimumHeight: 150

            clip: true

            Label {
                id: commandsoutput
                text: " -- "
            }

            contentY: contentHeight - height
        }

        Connections {
           target: bio_backend.protocol
           function onCommandResult(cmd, master_error, proto_output, slave_error) {
               cmd_timer.stop();
               commandsoutput.text = commandsoutput.text.concat("\nExecuted command: ", cmd);
               commandsoutput.text = commandsoutput.text.concat("\nmaster_error: ", master_error);
               commandsoutput.text = commandsoutput.text.concat("\nOutput of command: ", proto_output);
               commandsoutput.text = commandsoutput.text.concat("\nslave_error: ", slave_error);
               commandsoutput.text = commandsoutput.text.concat("\n -- ");
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

