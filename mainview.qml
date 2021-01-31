import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import Qt.labs.platform 1.1

import bioconverter 1.0

Item {
    RowLayout {
        anchors.fill: parent

        Button {
            Layout.alignment: Qt.AlignHCenter
            text: "Insert\nNew Container"
            font.bold: true
            font.pixelSize: 50
            onClicked: {
                bio_backend.protocol.runCommand(Protocol_MasterSlave.TRY_TO_INSERT_NEW_CONTAINER, []);
            }
        }

        Button {
            Layout.alignment: Qt.AlignHCenter
            text: "Containers\nInformation"
            font.bold: true
            font.pixelSize: 50
            onClicked: {
                bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_0TO5, []);
            }
        }

    }

    Connections {
        target: bio_backend.protocol
        function onCommandResult(cmd, master_error, proto_output, slave_error) {
            if (cmd == Protocol_MasterSlave.TRY_TO_INSERT_NEW_CONTAINER) {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SLAVE_ERROR)
                {
                    errordialog.title = "TRY_TO_INSERT_NEW_CONTAINER command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                    return;
                }
                thestackview.push("qrc:/insertcontainer.qml");
            }
            else if (cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_0TO5 ||
                     cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_6TO11 ||
                     cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_12TO17 ||
                     cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_18TO23)
            {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SLAVE_ERROR)
                {
                    errordialog.title = "GET_TAGS_NUMBER_AND_POSITION command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                    return;
                }
                if (cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_0TO5)
                {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_6TO11, []);
                }
                else if (cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_6TO11)
                {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_12TO17, []);
                }
                else if (cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_12TO17)
                {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_18TO23, []);
                }
                else if (cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_18TO23)
                {
                    thestackview.push("qrc:/infocontainers.qml");
                }
            }
        }
    }
}
