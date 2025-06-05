import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Atoms

Item {
    id: root

    property alias buttonHome: buttonHome
    property alias buttonSettings: buttonSettings
    property alias buttonInfo: buttonInfo

    Rectangle {

        color: "#333"
        Layout.fillWidth: true
        Layout.preferredHeight: 50

        Row {

            anchors.fill: parent

            ButtonIcon {
                id: buttonHome
                vIconName: "home"
            }

            ButtonIcon {
                id: buttonSettings
                vIconName: "settings"
            }

            ButtonIcon {
                id: buttonInfo
                vIconName: "info-circle"
            }
        }
    }
}
