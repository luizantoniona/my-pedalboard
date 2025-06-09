import QtQuick
import QtQuick.Controls

Item {
    id: root

    property alias button: button
    property alias outputSelector: outputSelector

    Column {
        spacing: 10

        ComboBox {
            id: outputSelector

            model: vOutputDevices
        }

        Button {
            id: button

            text: "Output"
        }
    }
}
