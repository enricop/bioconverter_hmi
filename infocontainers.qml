import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12

import bioconverter 1.0

Item {
    id: thecontainersinfo

    Button {
        text: "Back"
        onClicked: {
            thecontainersinfo.StackView.view.pop();
        }
    }

}
