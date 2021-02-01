import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import bioconverter 1.0

ToolBar {

    ColumnLayout {
        anchors.fill: parent

        GridLayout {
            columns: 3
            rows: 2

            Layout.alignment: Qt.AlignVCenter

            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                text: "Status:\n" + bio_backend.protocol.systeminfo1.status
                font.bold: true
                font.pixelSize: 25
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                text: "Current Action:\n" + bio_backend.protocol.systeminfo1.action
                font.bold: true
                font.pixelSize: 25
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                text: "Available Food:\n" + bio_backend.protocol.systeminfo1.foodAvailable
                font.bold: true
                font.pixelSize: 25
            }

            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                text: "Position to process:\n" + bio_backend.protocol.systeminfo1.positionToProcess
                font.bold: true
                font.pixelSize: 25
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                text: "Current Function:\n" + bio_backend.protocol.systeminfo1.functionInProgress
                font.bold: true
                font.pixelSize: 25
            }
            ToolButton {
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: true
                text: "Last Error Occurred:\n" + bio_backend.protocol.systeminfo1.errorOccurred
                font.bold: true
                font.pixelSize: 25
            }
        }
    }
}
