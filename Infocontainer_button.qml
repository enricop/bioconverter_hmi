import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import bioconverter 1.0

ItemDelegate {
    property int pos: -1
    property int tag: -1
    property bool isback: false

    hoverEnabled: true
    highlighted: hovered

    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
    Layout.preferredHeight: 55
    Layout.preferredWidth: 220

    opacity: highlighted ? 1.0 : (isback ? 1.0 : 0.8)

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
        border.width: highlighted ? 3 : 0
        color: {
            if (tag < 0)
                return "orange";

            return "green";
        }
    }

    onClicked: {
        bio_backend.protocol.runCommand(Protocol_MasterSlave.GET_SINGLE_CONTAINER_PARAMETERS1_BY_POS,
                                        [pos]);
    }

}
