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

        color: Colors.colors.background0
        anchors.fill: parent

        RowLayout {

            Layout.alignment: Qt.AlignCenter
            anchors.fill: parent

            ButtonIcon {
                id: buttonHome
                vIconName: "home"
                vIconSize: parent.height
            }

            ButtonIcon {
                id: buttonSettings
                vIconName: "settings"
                vIconSize: parent.height
            }

            ButtonIcon {
                id: buttonInfo
                vIconName: "info-circle"
                vIconSize: parent.height
            }
        }
    }
}
