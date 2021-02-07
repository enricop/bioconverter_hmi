import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import bioconverter 1.0

ItemDelegate {
    id: theinfocontainerbutton

    property int pos: -1
    property int tag: -1
    property bool isback: false

    hoverEnabled: true
    highlighted: hovered

    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
    Layout.preferredHeight: 55
    Layout.preferredWidth: 220

    contentItem: ColumnLayout {
        RowLayout {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Label {
                text: isback ? "Back" : "Front"
            }
            Label {
                text: " - "
            }
            Label {
                text: "Position: " + (pos + 1);
            }
        }

        Label {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            font.bold: true
            text: {
                if (tag < 0)
                    return "Empty";

                return "Tag: " + tag;
            }
        }

    }

    background: Rectangle {
        opacity: highlighted ? 1.0 : (isback ? 1.0 : 0.8)
        border.width: highlighted ? 3 : 0
        color: {
            if (tag < 0)
                return "orange";

            return "green";
        }
    }

    onClicked: {
        bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS, [pos]);
    }

    Connections {
        enabled: (thecontainersinfo.StackView.status == StackView.Active)
        target: bio_backend.protocol
        function onCommandResult(cmd, master_error, proto_output, slave_error) {
            if (cmd == Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS ||
                cmd == Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS2_BY_POS)
            {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SYSTEM_ERROR)
                {
                    errordialog.title = "GET_SINGLE_CONTAINER_PARAMETERS_BY_POS command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                    return;
                }

                if (proto_output[0] != pos)
                    return;

                if (cmd == Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS)
                {
                    bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS2_BY_POS, [pos]);
                }
                else if (cmd == Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS2_BY_POS)
                {
                    thecontainersinfo.StackView.view.push("qrc:/ContainerParameters.qml",
                                                    {
                                                        status: bio_backend.protocol.contp1.containers[pos].status,
                                                        foodtype: bio_backend.protocol.contp1.containers[pos].foodtype,
                                                        foodquantity: bio_backend.protocol.contp1.containers[pos].foodquantity,
                                                        foodcycletime: bio_backend.protocol.contp1.containers[pos].foodcycletime,
                                                        foodcycles: bio_backend.protocol.contp1.containers[pos].foodcycles,
                                                        remainingfoodcycletime: bio_backend.protocol.contp2.containers[pos].remainingfoodcycletime,
                                                        remainingfoodcycles: bio_backend.protocol.contp2.containers[pos].remainingfoodcycles,

                                                        thetag: tag,
                                                    });
                }
            }
        }
    }

}
