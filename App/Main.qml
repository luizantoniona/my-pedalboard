import QtQuick
import QtQuick.Window

Window {
    id: window

    title: "My Pedalboard"
    visibility: Window.Windowed
    visible: true

    ApplicationWindow {
        id: application

        anchors.fill: parent
    }
}
