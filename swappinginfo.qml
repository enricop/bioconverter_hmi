import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import bioconverter 1.0

ColumnLayout {
    id: theswappingpage
    spacing: 30

    readonly property int labelsfontsize: 30

    GridLayout {
        Layout.topMargin: 50
        Layout.alignment: Qt.AlignHCenter
        columns: 2

        Label {
            text: "Swap Procedure Cycle Duration:"
            font.pixelSize: labelsfontsize
        }
        Label {
            text: bio_backend.protocol.systeminfo2.swapCycleTime.getUTCDate()-1 + " days - " +
                  bio_backend.protocol.systeminfo2.swapCycleTime.getUTCHours() + " hours : " +
                  bio_backend.protocol.systeminfo2.swapCycleTime.getUTCMinutes() + " min";
            font.pixelSize: labelsfontsize
        }

        Label {
            text: "Swap Procedure Remaining Time:"
            font.pixelSize: labelsfontsize
        }
        Label {
            text: bio_backend.protocol.systeminfo2.remainingSwapCycleTime.getUTCDate()-1 + " days - " +
                  bio_backend.protocol.systeminfo2.remainingSwapCycleTime.getUTCHours() + " hours : " +
                  bio_backend.protocol.systeminfo2.remainingSwapCycleTime.getUTCMinutes() + " min";
            font.pixelSize: labelsfontsize
        }
    }

    Tumbler {
        id: swaptimestumbler
        model: 256
        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        wrap: false
    }
    Label {
        text: swaptimestumbler.currentIndex
        font.pixelSize: labelsfontsize
        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        horizontalAlignment: Text.AlignHCenter
    }
    Label {
        text: "New Swap Cycle Time (Hours)"
        font.pixelSize: labelsfontsize
        Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
        horizontalAlignment: Text.AlignHCenter
    }

    RowLayout {
        Layout.bottomMargin: 50
        Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
        spacing: 30

        Button {
            text: "Back"
            font.pixelSize: labelsfontsize
            onClicked: {
                theswappingpage.StackView.view.pop();
            }
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            text: "SET Selected Swap Time"
            font.pixelSize: labelsfontsize
            onClicked: {
                bio_backend.protocol.runCommand(Protocol_MasterSlave.SET_SYSTEM_PARAMETERS,
                                                [swaptimestumbler.currentIndex]);
            }
            Layout.alignment: Qt.AlignHCenter
        }
    }

    Connections {
        enabled: (theswappingpage.StackView.status == StackView.Active)
        target: bio_backend.protocol
        function onCommandResult(cmd, master_error, proto_output, slave_error) {
            if (cmd == Protocol_MasterSlave.SET_SYSTEM_PARAMETERS) {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SYSTEM_ERROR)
                {
                    errordialog.title = "SET_SINGLE_CONTAINER_PARAMETERS1 command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                }
                else
                {
                    successdialog.title = "SET_SYSTEM_PARAMETERS command successfull"
                    successtext.text = "\n";
                    successtext.text = successtext.text.concat("\nSWAP TIME SET SUCCESSFULLY TO: ",
                                                               bio_backend.protocol.setsys.newSwapCycleTime.getUTCDate()-1, " days - ",
                                                               bio_backend.protocol.setsys.newSwapCycleTime.getUTCHours(), " hours : ",
                                                               bio_backend.protocol.setsys.newSwapCycleTime.getUTCMinutes(), " min");
                    successdialog.open();
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_SYSTEM_INFO_2, []);
                }
            }
        }
    }




}
