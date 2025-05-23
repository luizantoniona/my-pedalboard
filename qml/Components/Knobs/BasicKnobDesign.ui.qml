import QtQuick 2.15

Item {
    id: root

    height: vSize
    width: vSize

    property alias knobArea: knobArea

    Rectangle {
        id: circle
        anchors.fill: parent
        color: vKnobColor
        radius: vSize / 2
    }

    //TODO Adjust the needle format

    Rectangle {
        id: needle
        width: 2
        height: vSize / 2
        radius: 1
        color: vNeedleColor

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: vSize * 0.1
    }

    MouseArea {
        id: knobArea
        anchors.fill: parent

        property bool dragging: false
        property real lastAngle: 0
    }
}
