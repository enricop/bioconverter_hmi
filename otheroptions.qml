import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import bioconverter 1.0

ColumnLayout {
    id: theotheroptionspage

    Button {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        text: "Erase System EEPROM Memory"
        font.pixelSize: 30
        onClicked: {
            bio_backend.protocol.runCommand(Protocol_MasterSlave.ERASE_EEPROM_RESET_SYSTEM, []);
        }
    }

    Button {
        text: "Back"
        font.pixelSize: 30
        onClicked: {
            theotheroptionspage.StackView.view.pop();
        }
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
    }

    Connections {
        enabled: (theotheroptionspage.StackView.status == StackView.Active)
        target: bio_backend.protocol
        function onCommandResult(cmd, master_error, proto_output, slave_error) {
            if (cmd == Protocol_MasterSlave.ERASE_EEPROM_RESET_SYSTEM) {
                if (master_error != Bioconverter.NO_MASTER_ERROR ||
                    slave_error != Bioconverter.NO_SYSTEM_ERROR)
                {
                    errordialog.title = "SET_SINGLE_CONTAINER_PARAMETERS1 command error"
                    errortext.text = "\n";
                    errortext.text = errortext.text.concat("\nmaster_error: ", master_error);
                    errortext.text = errortext.text.concat("\nslave_error: ", slave_error);
                    errordialog.open();
                }
                else
                {
                    successdialog.title = "ERASE_EEPROM_RESET_SYSTEM command successfull"
                    successtext.text = "\n";
                    successtext.text = successtext.text.concat("\MEMORY erased SUCCESSFULLY");
                    successdialog.open();
                    theotheroptionspage.StackView.view.pop(null);
                }
            }
        }
    }


}
