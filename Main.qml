import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Organisms
import Pages

Window {
    visible: true
    width: 1280
    height: 720
    title: "My Pedalboard"

    property int currentPage: 0

    enum PageEnum {
        Home,
        Settings
    }

    function updatePage(page) {
        stack.clear()
        switch (page) {
        case Main.PageEnum.Home:
            stack.push(pageHome)
            break
        case Main.PageEnum.Settings:
            stack.push(pageSettings)
            break
        }
    }

    ColumnLayout {
        anchors.fill: parent

        TopBar {
            id: topBar
            Layout.fillWidth: true
            onNavigate: updatePage(page)
        }

        StackView {
            id: stack
            Layout.fillWidth: true
            Layout.fillHeight: true

            initialItem: pageHome
        }
    }

    Component {
        id: pageHome

        PageHome {}
    }

    Component {
        id: pageSettings

        PageSettings {}
    }
}
