import QtQuick
import Audio

AudioOutputPanelDesign {
    id: root

    property var vOutputDevices: AudioDeviceManager.outputDevicesNames
}
