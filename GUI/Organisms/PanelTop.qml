import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Control
import Atoms
import Theme

Item {
    id: root

    required property AudioControl vAudioControl

    signal homeClicked
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
            font.pixelSize: Fonts.fontSizeMedium
            font.weight: Font.Bold
        }

        Item {
            Layout.fillWidth: true
        }

        ButtonIcon {
            vIcon: Icons.iconOutlinedPlayerPlay
            onClicked: function () {
                vAudioControl.start()
            }
        }

        ButtonIcon {
            vIcon: Icons.iconOutlinedPlayerStop
            onClicked: function () {
                vAudioControl.stop()
            }
        }

        Item {
            Layout.fillWidth: true
        }

        ButtonIcon {
            vIcon: Icons.iconOutlinedHome
            onClicked: function () {
                root.homeClicked()
            }
        }

        ButtonIcon {
            vIcon: Icons.iconOutlinedSettings
            onClicked: function () {
                root.settingsClicked()
            }
        }

        ButtonIcon {
            vIcon: Icons.iconOutlinedInfoCircle
            onClicked: function () {
                root.aboutClicked()
            }
        }
    }
}
