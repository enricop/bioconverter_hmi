import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import QtQml 2.15

GridLayout {
    id: theaboutpage
    columns: 2
    rows: 3
    columnSpacing: 20
    rowSpacing: 30

    Label {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "Bioconverter Control System Developer :"
        font.bold: true
        font.pixelSize: 25
    }
    Label {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "munteanu.evgheni@gmail.com"
        font.bold: true
        font.pixelSize: 20
    }

    Label {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "Bioconverter HMI Developer :"
        font.bold: true
        font.pixelSize: 25
    }
    Label {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "enrico.papi@gmail.com"
        font.bold: true
        font.pixelSize: 20
    }

    Label {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "HMI Version :"
        font.bold: true
        font.pixelSize: 25
    }
    Label {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: Qt.application.version
        font.bold: true
        font.pixelSize: 20
    }

    Button {
        text: "Back"
        font.pixelSize: 30
        onClicked: {
            theaboutpage.StackView.view.pop();
        }
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
    }

}
