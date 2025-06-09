import QtQuick
import QtQuick.Controls

Item {
    id: root

    property alias button: button
    property alias inputSelector: inputSelector

    Column {
        spacing: 10

        ComboBox {
            id: inputSelector

            model: vInputDevices
        }

        Button {
            id: button

            text: "Input"
        }
    }
}
