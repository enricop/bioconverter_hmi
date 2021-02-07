import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import bioconverter 1.0

Item {
    id: theinsertcontainerpage
    ColumnLayout {
        anchors.fill: parent

        Label {
            text: "Set New Container Parameters"
            Layout.alignment: Qt.AlignHCenter
            font.pixelSize: 40
        }

        GridLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            flow: GridLayout.TopToBottom
            columns: 4
            rows: 3

            columnSpacing: 30

            Slider {
                id: foodtypeslider
                orientation: Qt.Vertical
                from: 0
                to: 10
                snapMode: Slider.SnapAlways
                stepSize: 1.0
                Layout.alignment: Qt.AlignHCenter
            }
            Label {
                text: foodtypeslider.value
                font.pixelSize: 25
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
            }
            Label {
                text: "Food\nType"
                font.pixelSize: 25
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Tumbler {
                id: foodquantityslider
                model: 256
                Layout.alignment: Qt.AlignHCenter
                wrap: false
            }
            Label {
                text: foodquantityslider.currentIndex
                font.pixelSize: 25
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
            }
            Label {
                text: "Food\nQuantity (Units)"
                font.pixelSize: 25
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Slider {
                id: foodcyclesslider
                orientation: Qt.Vertical
                from: 1
                to: 10
                snapMode: Slider.SnapAlways
                stepSize: 1.0
                Layout.alignment: Qt.AlignHCenter
            }
            Label {
                text: foodcyclesslider.value
                font.pixelSize: 25
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
            }
            Label {
                text: "Food\nCycles Number"
                font.pixelSize: 25
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
            }

            Tumbler {
                id: foodtimeslider
                model: 256
                Layout.alignment: Qt.AlignHCenter
                wrap: false
            }
            Label {
                text: foodtimeslider.currentIndex
                font.pixelSize: 25
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
            }
            Label {
                text: "Food\nCycle Time (Hours)"
                font.pixelSize: 25
                Layout.alignment: Qt.AlignHCenter
                horizontalAlignment: Text.AlignHCenter
            }
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 30

            Button {
                text: "Back"
                font.pixelSize: 30
                onClicked: {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.ABORT_MANUAL_ACTION, []);
                }
                Layout.alignment: Qt.AlignHCenter
            }

            Button {
                text: "Insert Container"
                font.pixelSize: 30
                onClicked: {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.SET_SINGLE_CONTAINER_PARAMETERS1,
                                                    [1,
                                                     foodtypeslider.value,
                                                     foodquantityslider.currentIndex,
                                                     foodtimeslider.currentIndex,
                                                     foodcyclesslider.value]);
                }
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }

    Connections {
        enabled: (theinsertcontainerpage.StackView.status == StackView.Active)
        target: bio_backend.protocol
        function onCommandResult(cmd, master_error, proto_output, slave_error) {
            if (cmd == Protocol_MasterSlave.ABORT_MANUAL_ACTION) {
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
                theinsertcontainerpage.StackView.view.pop();
            }
        }
    }

    Connections {
        enabled: (theinsertcontainerpage.StackView.status == StackView.Active)
        target: bio_backend.protocol
        function onCommandResult(cmd, master_error, proto_output, slave_error) {
            if (cmd == Protocol_MasterSlave.SET_SINGLE_CONTAINER_PARAMETERS1) {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SLAVE_ERROR)
                {
                    errordialog.title = "SET_SINGLE_CONTAINER_PARAMETERS1 command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                }
                else
                {
                    successdialog.title = "SET_SINGLE_CONTAINER_PARAMETERS1 command successfull"
                    successtext.text = "\n";
                    successtext.text = successtext.text.concat("\nCONTAINER INSERTED SUCCESSFULLY WITH TAG: ", bio_backend.protocol.setp.newtag);
                    successtext.text = successtext.text.concat("\nRETURNED TAG: ", proto_output[0]);
                    successdialog.open();
                    theinsertcontainerpage.StackView.view.pop();
                }
            }
        }
    }

}
