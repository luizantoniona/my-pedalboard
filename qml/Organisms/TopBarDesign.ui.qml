import QtQuick
import QtQuick.Controls
import Atoms

Item {
    id: root

    Row {
        spacing: 12

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
