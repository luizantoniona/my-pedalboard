import QtQuick
import QtQuick.Controls
import Atoms

Item {
    id: root

    signal navigate(int page)

    Row {
        spacing: 12

        ButtonIcon {
            iconName: "home"
            onClicked: root.navigate(0)
        }

        ButtonIcon {
            iconName: "settings"
            onClicked: root.navigate(1)
        }

        ButtonIcon {
            iconName: "info-circle"
            onClicked: root.navigate(2)
        }
    }
}
