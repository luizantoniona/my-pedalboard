import QtQuick
import QtQuick.Controls

Item {

    Column {
        spacing: 10

        ComboBox {
            id: inputSelector
            model: ["TESTE INPUT", "TESTE1", "TESTE2"]
        }

        Button {
            text: "Start Input"
        }
    }
}
