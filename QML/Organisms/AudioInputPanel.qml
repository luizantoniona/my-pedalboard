import QtQuick
import Audio

AudioInputPanelDesign {
    id: root

    property var vInputDevices: AudioDeviceManager.inputDevicesNames

    button.onClicked: function () {
        var deviceName = inputSelector.currentText
        var device = AudioDeviceManager.inputDeviceByName(deviceName)
        AudioInputManager.setInputDevice(device)
    }
}
