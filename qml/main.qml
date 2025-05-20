import QtQuick 2.15
import QtQuick.Window 2.15
import Components 1.0

Window {
    height: 640
    title: qsTr("Hello World")
    visible: true
    width: 800

    Rectangle {
        id: testRectangle

        anchors.fill: parent

        Knob {
            id: knobtest

            anchors.centerIn: parent
        }
    }
}
