import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Atoms

Item {
    id: root

    property alias buttonAbout: buttonAbout
    property alias buttonHome: buttonHome
    property alias buttonSettings: buttonSettings

    RowLayout {
        Layout.fillHeight: true
        Layout.fillWidth: true
        spacing: 12

        ButtonIcon {
            id: buttonHome

            vIconName: Icons.iconOutlinedHome
            Layout.preferredWidth: root.height
            Layout.preferredHeight: root.height
        }

        ButtonIcon {
            id: buttonSettings

            vIconName: Icons.iconOutlinedSettings
            Layout.preferredWidth: root.height
            Layout.preferredHeight: root.height
        }

        ButtonIcon {
            id: buttonAbout

            vIconName: Icons.iconOutlinedInfoCircle
            Layout.preferredWidth: root.height
            Layout.preferredHeight: root.height
        }
    }
}
