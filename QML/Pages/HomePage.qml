import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Audio
import Pages

HomePageDesign {
    id: root

    ColumnLayout {
        anchors.fill: parent

    ComboBox {
        id: inputCombo
        model: AudioEngine.inputDevices
        onCurrentTextChanged: AudioEngine.setInputDevice(currentText)
    }

    ComboBox {
        id: outputCombo
        model: AudioEngine.outputDevices
        onCurrentTextChanged: AudioEngine.setOutputDevice(currentText)
    }

    Switch {
        text: "Bypass"
        onCheckedChanged: AudioEngine.setBypass(checked)
    }

    }

    HomePageControl {
        id: control
    }
}
