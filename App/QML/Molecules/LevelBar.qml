import QtQuick
import QtQuick.Controls
import Theme

Item {
    id: root

    signal valueChanged(real value)

    property color vColorBackground: Colors.background2
    property color vColorBorder: Colors.primary
    property color vColorFill: Colors.primary
    property color vColorHandle: Colors.primary

    property real vValue: 0
    property real vMinimum: 0
    property real vMaximum: 1

    readonly property real vClampedValue: Math.min(Math.max(vValue, vMinimum), vMaximum)
    readonly property real vRatio: vMaximum > vMinimum ? (vClampedValue - vMinimum) / (vMaximum - vMinimum) : 0

    property int vOrientation: Qt.Vertical

    property real vWidth: 160
    property real vHeight: 20

    width: vOrientation === Qt.Horizontal ? vWidth : vHeight
    height: vOrientation === Qt.Horizontal ? vHeight : vWidth

    function updateValue(mouseX, mouseY) {
        var r

        if (vOrientation === Qt.Horizontal) {
            r = mouseX / width
        } else {
            r = 1.0 - (mouseY / height)
        }

        r = Math.max(0, Math.min(1, r))
        vValue = vMinimum + r * (vMaximum - vMinimum)
        valueChanged(vValue)
    }

    Rectangle {
        id: bar
        anchors.fill: parent
        color: vColorBackground
        radius: Radius.radius4
        border.color: vColorBorder
        border.width: 1
    }

    Rectangle {
        id: fill
        anchors.left: bar.left
        anchors.bottom: bar.bottom

        width: vOrientation === Qt.Horizontal ? bar.width * vRatio : bar.width
        height: vOrientation === Qt.Horizontal ? bar.height : bar.height * vRatio

        color: vColorFill
        radius: Radius.radius4
    }

    Rectangle {
        id: handle
        width: vHeight
        height: vHeight
        radius: Radius.radius4

        color: vColorHandle
        border.color: vColorBorder
        border.width: 1

        x: vOrientation === Qt.Horizontal ? (bar.width * vRatio) - width / 2 : (bar.width - width) / 2
        y: vOrientation === Qt.Horizontal ? (bar.height - height) / 2 : (bar.height * (1 - vRatio)) - height / 2
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPressed: function (mouse) {
            root.updateValue(mouse.x, mouse.y)
        }
        onPositionChanged: function (mouse) {
            if (pressed) {
                root.updateValue(mouse.x, mouse.y)
            }
        }
    }
}
