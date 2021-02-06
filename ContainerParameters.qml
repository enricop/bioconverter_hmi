import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import bioconverter 1.0

RowLayout {
    id: thecontainerparameters
    property var status
    property var foodtype
    property var foodquantity
    property var foodcycletime
    property var foodcycles
    property var remainingfoodcycletime
    property var remainingfoodcycles

    GridLayout {
        columns: 2

        Label {
            text: "Status"
        }
        Label {
            text: status
        }

        Label {
            text: "Food Type"
        }
        Label {
            text: foodtype
        }

        Label {
            text: "Food Quantity"
        }
        Label {
            text: foodquantity
        }

        Label {
            text: "Food Cycle Duration"
        }
        Label {
            text: foodcycletime
        }

        Label {
            text: "Food Cycles Number"
        }
        Label {
            text: foodcycles
        }

        Label {
            text: "Remaining Food Cycle Time"
        }
        Label {
            text: foodcycles
        }

        Label {
            text: "Remaining Food Cycles"
        }
        Label {
            text: foodcycles
        }
    }

    ColumnLayout {
        spacing: 15
        Button {
            text: "Show Container"
            font.pixelSize: 30
            onClicked: {
                //thecontainersinfo.StackView.view.pop();
            }
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
        Button {
            text: "Delete Container"
            font.pixelSize: 30
            onClicked: {
                //thecontainersinfo.StackView.view.pop();
            }
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
        Button {
            text: "Back"
            font.pixelSize: 30
            onClicked: {
                thecontainerparameters.StackView.view.pop();
            }
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        }
    }



}
