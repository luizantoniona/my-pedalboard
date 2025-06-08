import QtQuick
import Audio

AudioInputPanelDesign {
    id: root

    property var vInputDevices: AudioDeviceManager.inputDevicesNames
}
