import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Molecules
import Theme

Item {
    id: root

    Rectangle {
        id: background

        anchors.fill: parent
        color: Colors.surface
        border.color: Colors.border
        border.width: 1
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: Spacing.spacing4
        anchors.rightMargin: Spacing.spacing4
        spacing: Spacing.spacing4

        Item {
            Layout.fillWidth: true
        }
    }
}
