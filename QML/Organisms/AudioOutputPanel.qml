import QtQuick
import Audio

AudioOutputPanelDesign {
    id: root

    property var vOutputDevices: AudioDeviceManager.outputDevicesNames

    button.onClicked: function () {
        var deviceName = outputSelector.currentText
        var device = AudioDeviceManager.outputDeviceByName(deviceName)
        AudioOutputManager.setOutputDevice(device)
    }
}
