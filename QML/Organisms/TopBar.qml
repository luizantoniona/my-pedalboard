import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Molecules
import Theme

Item {
    id: root

    signal aboutClicked
    signal settingsClicked

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

        Text {
            text: "My Pedalboard"
            color: Colors.textPrimary
            font.pixelSize: 16
            font.weight: Font.Bold
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            id: settingsButton

            implicitWidth: 32
            implicitHeight: 32
            text: "⚙"

            onClicked: root.settingsClicked()
        }

        Button {
            id: aboutButton

            implicitWidth: 32
            implicitHeight: 32
            text: "ℹ️"

            onClicked: root.aboutClicked()
        }
    }
}
