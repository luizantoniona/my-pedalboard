import QtQuick
import QtQuick.Controls
import Theme

ComboBox {
    id: combo

    property color contentColor: Colors.textPrimary
    property color backgroundColor: Colors.surface
    property color borderColor: Colors.border
    property real cornerRadius: 12
    property real itemHeight: 40

    implicitHeight: 44
    width: 180

    background: Rectangle {
        color: combo.backgroundColor
        radius: combo.cornerRadius
        border.color: combo.borderColor
        border.width: 1
    }

    contentItem: Text {
        anchors.fill: parent
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: 14
        anchors.rightMargin: 26
        text: combo.currentText
        color: combo.contentColor
        elide: Text.ElideRight
    }

    indicator: Item {
        width: 24
        height: 24
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        Text {
            anchors.centerIn: parent
            text: "\u25BE"
            color: combo.contentColor
            font.pixelSize: 14
        }
    }
}
