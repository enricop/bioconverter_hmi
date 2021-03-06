import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import bioconverter 1.0

ColumnLayout {
    id: theserialmngpage
    spacing: 40

    Button {
        Layout.topMargin: 40
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "Back"
        font.pixelSize: 15
        onClicked: {
            theserialmngpage.StackView.view.pop();
        }
    }

    Button {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "Close Serial Port"
        onClicked: {
            var ret = bio_backend.serialport.closeSerialPort();
            if (ret) {
                theheader.pollingenabled = false;
            }
        }
    }

    Button {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "Open Serial Port"
        onClicked: {
            var ret = bio_backend.serialport.openSerialPort();
            if (ret) {
                theheader.pollingenabled = true;
            }
        }
    }

    Flickable {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        contentWidth: serialcontroloutput.width
        contentHeight: serialcontroloutput.height
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true

        Label {
            id: serialcontroloutput
            text: bio_backend.serialport.controlOutput
        }
        contentY: contentHeight - height
    }
}
