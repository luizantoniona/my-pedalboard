import QtQuick
import Molecules

NavigationBarDesign {
    id: root

    signal navigate(int page)

    buttonAbout.onClicked: function () {
        root.navigate(0);
    }

    buttonHome.onClicked: function () {
        root.navigate(1);
    }

    buttonSettings.onClicked: function () {
        root.navigate(2);
    }

    NavigationBarControl {
        id: control

    }
}
