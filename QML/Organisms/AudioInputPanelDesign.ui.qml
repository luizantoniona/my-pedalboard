import QtQuick
import QtQuick.Controls

Item {
    id: root

    Column {
        spacing: 10

        ComboBox {
            id: inputSelector

            model: vInputDevices
        }

        Button {
            text: "Input"
        }
    }
}
