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
        Layout.alignment: Qt.AlignHCenter
        anchors.fill: parent

        ButtonIcon {
            id: buttonHome

            vIconName: "outlined/home"
            vIconSize: root.height
        }

        ButtonIcon {
            id: buttonSettings

            vIconName: "outlined/settings"
            vIconSize: root.height
        }

        ButtonIcon {
            id: buttonAbout

            vIconName: "outlined/info-circle"
            vIconSize: root.height
        }
    }
}
