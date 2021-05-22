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

            rowSpacing: 15
            columnSpacing: 15

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
                text: foodcycletime.getUTCDate()-1 + " days - " + foodcycletime.getUTCHours() + " hours : " + foodcycletime.getUTCMinutes() + " mins";
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
                text: remainingfoodcycletime.getUTCDate()-1 + " days - " + remainingfoodcycletime.getUTCHours() + " hours : " + remainingfoodcycletime.getUTCMinutes() + " mins";
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
            spacing: 25
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Button {
                id: showbutton
                enabled: status !== 0 && containerInplace
                text: "Show Container to Food Platform"
                font.pixelSize: 30
                onClicked: {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.TRY_TO_SHOW_CONTAINER, [thetag]);
                }
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
            Button {
                id: gobackbutton
                enabled: containerShown
                text: "Container Go Back"
                font.pixelSize: 30
                onClicked: {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.SHOW_CONTAINER_GO_BACK, []);
                }
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
            Button {
                id: deletebutton
                enabled: status !== 0 && containerInplace
                text: "Delete Container from Memory"
                font.pixelSize: 30
                onClicked: {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.CANCEL_CONTAINER_BY_TAG, [thetag]);
                }
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
            Button {
                id: endrearingbutton
                enabled: containerShown
                text: "Move Container to Exit Platform"
                font.pixelSize: 30
                onClicked: {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.END_REARING_CYCLE, []);
                }
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            }
        }
    }

    Button {
        id: backbutton
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "Back"
        font.pixelSize: 30
        onClicked: {
            bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_0TO5, []);
        }
    }

    property bool containerInplace:
        (bio_backend.protocol.systeminfo1.status == Get_System_Info_1.SYS_STS_AUTO_TASK_NOT_RUNNING &&
         bio_backend.protocol.systeminfo1.function1InProgress == "NO_RUN" &&
         bio_backend.protocol.systeminfo1.function2InProgress == "NO_RUN")

    property bool containerShown:
        (bio_backend.protocol.systeminfo1.function1InProgress == "SYS_ACTION_USER_SHOW_CONTAINER" &&
         bio_backend.protocol.systeminfo1.function2InProgress == "NO_RUN" &&
         bio_backend.protocol.systeminfo1.function3InProgress == "NO_RUN" &&
         bio_backend.protocol.systeminfo1.function4InProgress == "NO_RUN")

    Connections {
        enabled: (thecontainerparameters.StackView.status == StackView.Active)
        target: bio_backend.protocol
        function onCommandResult(cmd, master_error, proto_output, slave_error) {
            if (cmd == Protocol_MasterSlave.CANCEL_CONTAINER_BY_TAG) {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SYSTEM_ERROR)
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
                    slave_error != Bioconverter.NO_SYSTEM_ERROR)
                {
                    errordialog.title = "TRY_TO_SHOW_CONTAINER command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                    return;
                }
            }
            else if (cmd == Protocol_MasterSlave.SHOW_CONTAINER_GO_BACK)
            {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SYSTEM_ERROR)
                {
                    errordialog.title = "SHOW_CONTAINER_GO_BACK command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                    return;
                }
            }
            else if (cmd == Protocol_MasterSlave.END_REARING_CYCLE)
            {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SYSTEM_ERROR)
                {
                    errordialog.title = "END_REARING_CYCLE command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                    return;
                }
                thecontainerparameters.StackView.view.pop(null);
            }
            else if (cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_0TO5 ||
                     cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_6TO11 ||
                     cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_12TO17 ||
                     cmd == Protocol_MasterSlave.GET_TAGS_NUMBER_AND_POSITION_18TO23)
            {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SYSTEM_ERROR)
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
