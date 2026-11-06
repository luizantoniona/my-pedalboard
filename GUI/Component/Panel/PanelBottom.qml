import QtQuick
import QtQuick.Layouts
import MyPedalboardGUIComponents

Rectangle {
    id: root

    default property alias vContent: content.data

    property real vHeight: 40

    Layout.fillWidth: true
    Layout.preferredHeight: vHeight

    color: Colors.background2

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: Borders.thin
        color: Colors.border
    }

    RowLayout {
        id: content

        anchors.fill: parent
        anchors.leftMargin: Spaces.spacing4
        anchors.rightMargin: Spaces.spacing4

        spacing: Spaces.spacing4
    }
}
