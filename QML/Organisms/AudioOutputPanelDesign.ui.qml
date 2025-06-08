import QtQuick
import QtQuick.Controls

Item {
    id: root

    Column {
        spacing: 10

        ComboBox {
            id: outputSelector

            model: deviceManager.outputDevicesNames
        }

        Button {
            text: "Output"
        }
    }
}
