import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import Atoms
import Organisms
import Pages

Window {
    id: window

    enum PageEnum {
        About,
        Home,
        Settings
    }

    function updatePage(page) {
        stack.clear();
        switch (page) {
        case Main.PageEnum.Home:
            stack.push(pageHome);
            break;
        case Main.PageEnum.Settings:
            stack.push(pageSettings);
            break;
        case Main.PageEnum.About:
            stack.push(pageAbout);
            break;
        }
    }

    title: "My Pedalboard"
    visibility: "FullScreen"
    visible: true

    Rectangle {
        id: background

        anchors.fill: parent
        color: Colors.color.background1

        ColumnLayout {
            anchors.fill: parent
            spacing: Spaces.spacing0

            StatusPanel {
                id: statusPanel

                Layout.fillWidth: true
                Layout.preferredHeight: 0.1 * background.height
            }

            StackView {
                id: stack

                Layout.fillHeight: true
                Layout.fillWidth: true
                initialItem: pageHome
            }
        }
    }

    Component {
        id: pageHome

        HomePage {
        }
    }

    Component {
        id: pageSettings

        SettingsPage {
        }
    }

    Component {
        id: pageAbout

        AboutPage {
        }
    }
}
