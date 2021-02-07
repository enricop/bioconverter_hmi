import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import QtQml 2.15

import bioconverter 1.0

ColumnLayout {
    id: thecontainerparameters
    property var status
    property var foodtype
    property var foodquantity
    property var foodcycletime
    property var foodcycles
    property var remainingfoodcycletime
    property var remainingfoodcycles

    property int thetag

    property int labelsfontsize: 20

    RowLayout {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        spacing: 50

        GridLayout {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            columns: 2

            Label {
                text: "Tag:"
                font.pixelSize: labelsfontsize
            }
            Label {
                text: thetag
                font.pixelSize: labelsfontsize
            }

            Label {
                text: "Status:"
                font.pixelSize: labelsfontsize
            }
            Label {
                text: {
                    if (status == 0)
                        return "NOT_INSERTED";
                    else if (status == 1)
                        return "INSERTED";
                    else if (status == 2)
                        return "MOVING";
                    else
                        return "UNKNOWN";
                }

                font.pixelSize: labelsfontsize
            }

            Label {
                text: "Food Type:"
                font.pixelSize: labelsfontsize
            }
            Label {
                text: foodtype
                font.pixelSize: labelsfontsize
            }

            Label {
                text: "Food Quantity:"
                font.pixelSize: labelsfontsize
            }
            Label {
                text: foodquantity
                font.pixelSize: labelsfontsize
            }

            Label {
                text: "Food Cycle Duration:"
                font.pixelSize: labelsfontsize
            }
            Label {
                text: foodcycletime.getDate()-1 + " days - " + foodcycletime.getHours() + " hours : " + foodcycletime.getMinutes() + " mins";
                font.pixelSize: labelsfontsize
            }

            Label {
                text: "Food Cycles Number:"
                font.pixelSize: labelsfontsize
            }
            Label {
                text: foodcycles
                font.pixelSize: labelsfontsize
            }

            Label {
                text: "Remaining Food Cycle Time:"
                font.pixelSize: labelsfontsize
            }
            Label {
                text: remainingfoodcycletime.getDate()-1 + " days - " + remainingfoodcycletime.getHours() + " hours : " + remainingfoodcycletime.getMinutes() + " mins";
                font.pixelSize: labelsfontsize
            }

            Label {
                text: "Remaining Food Cycles:"
                font.pixelSize: labelsfontsize
            }
            Label {

                text: remainingfoodcycles
                font.pixelSize: labelsfontsize
            }
        }

        ColumnLayout {
            spacing: 15
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Button {
                text: "Show Container"
                font.pixelSize: 30
                onClicked: {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.TRY_TO_SHOW_CONTAINER, [thetag]);
                }
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
            Button {
                id: gobackbutton
                enabled: false
                text: "Container Go Back"
                font.pixelSize: 30
                onClicked: {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.SHOW_CONTAINER_GO_BACK, []);
                }
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
            Button {
                text: "Delete Container"
                font.pixelSize: 30
                onClicked: {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.CANCEL_CONTAINER_BY_TAG, [thetag]);
                }
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
        }
    }

    Button {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "Back"
        font.pixelSize: 30
        onClicked: {
            thecontainerparameters.StackView.view.pop();
        }
    }

    Connections {
        enabled: (thecontainerparameters.StackView.status == StackView.Active)
        target: bio_backend.protocol
        function onCommandResult(cmd, master_error, proto_output, slave_error) {
            if (cmd == Protocol_MasterSlave.CANCEL_CONTAINER_BY_TAG) {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SLAVE_ERROR)
                {
                    errordialog.title = "CANCEL_CONTAINER_BY_TAG command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                    return;
                }
                bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_0TO5, []);
            }
            else if (cmd == Protocol_MasterSlave.TRY_TO_SHOW_CONTAINER)
            {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SLAVE_ERROR)
                {
                    errordialog.title = "TRY_TO_SHOW_CONTAINER command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                    return;
                }
                gobackbutton.enabled = true;
            }
            else if (cmd == Protocol_MasterSlave.SHOW_CONTAINER_GO_BACK)
            {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SLAVE_ERROR)
                {
                    errordialog.title = "SHOW_CONTAINER_GO_BACK command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                    return;
                }
                gobackbutton.enabled = false;
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
                    thecontainerparameters.StackView.view.pop();
                }
            }
        }
    }


}
