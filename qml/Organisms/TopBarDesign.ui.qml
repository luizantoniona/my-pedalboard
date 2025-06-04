import QtQuick
import QtQuick.Controls
import Atoms

Item {
    id: root

    property alias buttonHome: buttonHome
    property alias buttonSettings: buttonSettings
    property alias buttonInfo: buttonInfo

    Row {

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
