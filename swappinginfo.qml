import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import bioconverter 1.0

ColumnLayout {
    id: theserialmngpage
    spacing: 40

    readonly property int labelsfontsize: 30

    Button {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "Back"
        font.pixelSize: 15
        onClicked: {
            theserialmngpage.StackView.view.pop();
        }
    }

    GridLayout {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        columns: 2

        Label {
            text: "Swap Procedure Cycle Duration:"
            font.pixelSize: labelsfontsize
        }
        Label {
            text: bio_backend.protocol.systeminfo2.swapCycleTime.getDate()-1 + " days - " +
                  bio_backend.protocol.systeminfo2.swapCycleTime.getHours() + " hours : " +
                  bio_backend.protocol.systeminfo2.swapCycleTime.getMinutes() + " min";
            font.pixelSize: labelsfontsize
        }

        Label {
            text: "Swap Procedure Remaining Time:"
            font.pixelSize: labelsfontsize
        }
        Label {
            text: bio_backend.protocol.systeminfo2.remainingSwapCycleTime.getDate()-1 + " days - " +
                  bio_backend.protocol.systeminfo2.remainingSwapCycleTime.getHours() + " hours : " +
                  bio_backend.protocol.systeminfo2.remainingSwapCycleTime.getMinutes() + " min";
            font.pixelSize: labelsfontsize
        }

    }
}
