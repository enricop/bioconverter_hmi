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

        Flickable {
            width: parent.width
            height: 200
            contentWidth: 200
            contentHeight: writeoutput.height

            Layout.fillWidth: true
            Layout.maximumHeight: 200
            Layout.minimumHeight: 200

            clip: true

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
                    id: writeoutput
                    text: bio_backend.serialport.writeOutput
                }
                Label {
                    text: bio_backend.serialport.readOutput
                }

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
                param: function (){
                    return [];
                }
            }
            ListElement {
                name: "GET_SYSTEM_INFO_2"
                cmd: Protocol_MasterSlave.GET_SYSTEM_INFO_2
                param: function (){
                    return [];
                }
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_0TO5"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_0TO5
                param: function (){
                    return [];
                }
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_6TO11"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_6TO11
                param: function (){
                    return [];
                }
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_12TO17"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_12TO17
                param: function (){
                    return [];
                }
            }
            ListElement {
                name: "GET_TAGS_NUMBER_AND_POSITION_18TO23"
                cmd: Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_18TO23
                param: function (){
                    return [];
                }
            }
            ListElement {
                name: "GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS"
                cmd: Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS
                param: function (){
                    return [2];
                }
            }
            ListElement {
                name: "GET_SINGLE_CONTAINER_PARAMETERS2_BY_POS"
                cmd: Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS2_BY_POS
                param: function (){
                    return [2];
                }
            }
            ListElement {
                name: "TRY_TO_INSERT_NEW_CONTAINER"
                cmd: Protocol_MasterSlave.TRY_TO_INSERT_NEW_CONTAINER
                param: function (){
                    return [];
                }
            }
            ListElement {
                name: "SET_SINGLE_CONTAINER_PARAMETERS1"
                cmd: Protocol_MasterSlave.SET_SINGLE_CONTAINER_PARAMETERS1
                param: function (){
                    return [1, 1, 1, 1, 1];
                }
            }
            ListElement {
                name: "ERASE_EEPROM_RESET_SYSTEM"
                cmd: Protocol_MasterSlave.ERASE_EEPROM_RESET_SYSTEM
                param: function (){
                    return [];
                }
            }
            ListElement {
                name: "TRY_TO_SHOW_CONTAINER"
                cmd: Protocol_MasterSlave.TRY_TO_SHOW_CONTAINER
                param: function (){
                    return [101];
                }
            }
            ListElement {
                name: "SHOW_CONTAINER_GO_BACK"
                cmd: Protocol_MasterSlave.SHOW_CONTAINER_GO_BACK
                param: function (){
                    return [];
                }
            }
            ListElement {
                name: "MANAGE_ERROR"
                cmd: Protocol_MasterSlave.MANAGE_ERROR
                param: function (){
                    return [];
                }
            }
            ListElement {
                name: "DELETE_ALL_ERRORS"
                cmd: Protocol_MasterSlave.DELETE_ALL_ERRORS
                param: function (){
                    return [];
                }
            }
            ListElement {
                name: "CANCEL_CONTAINER_BY_TAG"
                cmd: Protocol_MasterSlave.CANCEL_CONTAINER_BY_TAG
                param: function (){
                    return [101];
                }
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
                        bio_backend.protocol.runCommand(cmd, param);
                    }
                }
            }
        }

        Connections {
           target: bio_backend.protocol
           function onCommandResult(cmd, master_error, proto_output, slave_error) {
               cmd_timer.stop();
               console.log("Executed command: ", cmd);
               console.log("master_error: ", master_error);
               console.log("Output of command: ", proto_output);
               console.log("slave_error: ", slave_error);
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

