import QtQuick
import QtQuick.Window
import Application

Window {
    id: window

    title: "My Pedalboard"
    visibility: Window.FullScreen
    visible: true

    ApplicationWindow {
        id: application

        anchors.fill: parent
    }
}
