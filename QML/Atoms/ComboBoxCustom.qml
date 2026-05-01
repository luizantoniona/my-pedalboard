import QtQuick
import QtQuick.Controls
import Theme

ComboBox {
    id: combo

    property color vColorBackground: Colors.background2
    property color vColorBorder: Colors.primary
    property color vColor: Colors.primary

    property real vWidth: 80
    property real vHeight: 40

    width: vWidth
    implicitHeight: vHeight

    background: Rectangle {
        color: vColorBackground
        radius: Radius.radius8
        border.color: vColorBorder
        border.width: 1
    }

    contentItem: Text {
        anchors.fill: parent
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: Spacing.spacing4
        anchors.rightMargin: Spacing.spacing4
        text: combo.currentText
        color: vColor
        elide: Text.ElideRight
    }

    indicator: Item {
        width: 24
        height: 24
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        Text {
            anchors.centerIn: parent
            color: vColor
        }
    }
}
