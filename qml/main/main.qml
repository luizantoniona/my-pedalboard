import QtQuick 2.15
import QtQuick.Window 2.15
import Components 1.0

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
