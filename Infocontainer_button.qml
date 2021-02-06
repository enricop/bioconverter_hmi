import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import bioconverter 1.0

ItemDelegate {
    property int pos: -1
    property int tag: -1
    property bool isback: false

    width: 200
    height: 50

    opacity: isback ? 1.0 : 0.8

    contentItem: ColumnLayout {
        RowLayout {

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
            Layout.alignment: Qt.AlignHCenter
            text: {
                if (tag < 0)
                    return "Empty";

                return "Tag: " + tag;
            }
        }

    }

    background: Rectangle {
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
