import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Control
import Organisms
import Pages
import Theme

Item {
    id: root

    function updatePage(page) {
        stack.clear()

        switch (page) {
        case "home":
            stack.push(pageHome)
            break
        case "settings":
            stack.push(pageSettings)
            break
        case "about":
            stack.push(pageAbout)
            break
        }
    }

    Rectangle {
        id: background

        anchors.fill: parent
        color: Colors.background1
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TopBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 64

            onAboutClicked: function () {
                root.updatePage("about")
            }
            onSettingsClicked: function () {
                root.updatePage("settings")
            }
        }

        StackView {
            id: stack

            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        BottomBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 128
        }
    }

    Component.onCompleted: {
        root.updatePage("home")
    }

    Component {
        id: pageHome

        HomePage {}
    }

    Component {
        id: pageSettings

        SettingsPage {}
    }

    Component {
        id: pageAbout

        AboutPage {}
    }

    AudioControl {
        id: audioControl
    }
}
