import QtQuick

TopBarDesign {
    id: root

    signal navigate(int page)

    buttonHome.onClicked: function () {
        root.navigate(0)
    }

    buttonSettings.onClicked: function () {
        root.navigate(1)
    }
}
