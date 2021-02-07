import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import bioconverter 1.0

ColumnLayout {
    id: erroroccurredpage

    spacing: 40

    Label {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        font.bold: true
        font.pixelSize: 25
        text: "Bioconverted System Error Occurred:"
    }

    Label {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        font.bold: true
        font.pixelSize: 25
        text: bio_backend.protocol.systeminfo1.errorOccurredStr
    }

    Button {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "Manage Error"
        font.pixelSize: 30
        onClicked: {
            bio_backend.protocol.runCommand(Protocol_MasterSlave.MANAGE_ERROR, []);
        }
    }

    Button {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "Delete Error"
        font.pixelSize: 30
        onClicked: {
            bio_backend.protocol.runCommand(Protocol_MasterSlave.DELETE_ALL_ERRORS, []);
        }
    }

    Connections {
        enabled: (erroroccurredpage.StackView.status == StackView.Active)
        target: bio_backend.protocol
        function onCommandResult(cmd, master_error, proto_output, slave_error) {
            if (cmd == Protocol_MasterSlave.MANAGE_ERROR ||
                cmd == Protocol_MasterSlave.MANAGE_ERROR) {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SLAVE_ERROR)
                {
                    errordialog.title = "MANAGE_ERROR or MANAGE_ERROR commands error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                    return;
                }

                erroroccurredpage.StackView.view.pop();
            }
        }
    }

}
