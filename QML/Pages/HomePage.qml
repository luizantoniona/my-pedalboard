import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Organisms
import Theme

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: Colors.background1
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
