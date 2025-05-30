import QtQuick
import QtQuick.Window
import Components

Window {
    height: 640
    title: qsTr("My Pedalboard")
    visible: true
    width: 800

    Row {
        anchors.fill: parent

        MeterKnob {
            vSize: 250
        }

        BasicKnob {
            vSize: 250
        }
    }
}
