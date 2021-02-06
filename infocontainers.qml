import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import bioconverter 1.0

Item {
    id: thecontainersinfo

    readonly property var infobackmodel: [
        { tagprop: bio_backend.protocol.tags2.pos10, posprop: 10 },
        { tagprop: bio_backend.protocol.tags2.pos8, posprop: 8 },
        { tagprop: bio_backend.protocol.tags2.pos6, posprop: 6 },
        { tagprop: bio_backend.protocol.tags1.pos4, posprop: 4 },
        { tagprop: bio_backend.protocol.tags1.pos2, posprop: 2 },
        { tagprop: bio_backend.protocol.tags1.pos0, posprop: 0 },
        { tagprop: bio_backend.protocol.tags4.pos22, posprop: 22 },
        { tagprop: bio_backend.protocol.tags4.pos20, posprop: 20 },
        { tagprop: bio_backend.protocol.tags4.pos18, posprop: 18 },
        { tagprop: bio_backend.protocol.tags3.pos16, posprop: 16 },
        { tagprop: bio_backend.protocol.tags3.pos14, posprop: 14 },
        { tagprop: bio_backend.protocol.tags3.pos12, posprop: 12 },
    ];

    readonly property var infofrontmodel: [
        { tagprop: bio_backend.protocol.tags2.pos11, posprop: 11 },
        { tagprop: bio_backend.protocol.tags2.pos9, posprop: 9},
        { tagprop: bio_backend.protocol.tags2.pos7, posprop: 7 },
        { tagprop: bio_backend.protocol.tags1.pos5, posprop: 5 },
        { tagprop: bio_backend.protocol.tags1.pos3, posprop: 3 },
        { tagprop: bio_backend.protocol.tags1.pos1, posprop: 1 },
        { tagprop: bio_backend.protocol.tags4.pos23, posprop: 23 },
        { tagprop: bio_backend.protocol.tags4.pos21, posprop: 21 },
        { tagprop: bio_backend.protocol.tags4.pos19, posprop: 19 },
        { tagprop: bio_backend.protocol.tags3.pos17, posprop: 17 },
        { tagprop: bio_backend.protocol.tags3.pos15, posprop: 15 },
        { tagprop: bio_backend.protocol.tags3.pos13, posprop: 13 },
    ];

    ColumnLayout {
        anchors.fill: parent

        spacing: 7

        Item {
            Layout.fillHeight: true
            Layout.fillWidth: true

            GridLayout {
                anchors.fill: parent
                anchors.bottomMargin: 30
                anchors.leftMargin: 150
                flow: GridLayout.TopToBottom
                columns: 2
                rows:  6
                columnSpacing: 100
                Repeater {
                    model: infobackmodel
                    delegate: Infocontainer_button {
                        isback: true
                        tag: modelData.tagprop
                        pos: modelData.posprop
                        level: modelData.posprop/2
                    }
                }
            }

            GridLayout {
                anchors.fill: parent
                anchors.topMargin: 30
                anchors.rightMargin: 150
                flow: GridLayout.TopToBottom
                columns: 2
                rows:  6
                columnSpacing: 100
                Repeater {
                    model: infofrontmodel
                    delegate: Infocontainer_button {
                        isback: false
                        tag: modelData.tagprop
                        pos: modelData.posprop
                    }
                }
            }
        }

        Button {
            text: "Back"
            font.pixelSize: 30
            onClicked: {
                thecontainersinfo.StackView.view.pop();
            }
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
    }

    Connections {
        enabled: (thecontainersinfo.StackView.status == StackView.Active)
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
