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
                    theinsertcontainerpage.StackView.view.pop();
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
                                                     foodcyclesslider.value,
                                                     foodtimeslider.currentIndex]);
                    theinsertcontainerpage.StackView.view.pop();
                }
                Layout.alignment: Qt.AlignHCenter
            }


        }

    }
}
