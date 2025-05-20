import QtQuick 2.15
import QtQuick.Window 2.15
import Component 1.0

Window {
    width: 800
    height: 640
    visible: true
    title: qsTr("Hello World")

    Rectangle {
        id: testRectangle

        anchors.fill: parent

        Knob {
            id: knobtest
            anchors.centerIn: parent
        }
    }
}
