import QtQuick
import QtQuick.Controls

Item {
    id: root

    Column {
        spacing: 10

        ComboBox {
            id: inputSelector

            model: deviceManager.inputDevicesNames
        }

        Button {
            text: "Input"
        }
    }
}
