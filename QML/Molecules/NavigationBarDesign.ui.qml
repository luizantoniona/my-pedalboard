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

            vIconName: "outlined/home"
            Layout.preferredWidth: root.height
            Layout.preferredHeight: root.height
        }

        ButtonIcon {
            id: buttonSettings

            vIconName: "outlined/settings"
            Layout.preferredWidth: root.height
            Layout.preferredHeight: root.height
        }

        ButtonIcon {
            id: buttonAbout

            vIconName: "outlined/info-circle"
            Layout.preferredWidth: root.height
            Layout.preferredHeight: root.height
        }
    }
}
