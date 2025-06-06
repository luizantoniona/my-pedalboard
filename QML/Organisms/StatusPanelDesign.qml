import QtQuick
import QtQuick.Layouts
import Atoms
import Molecules

Item {
    id: root

    property alias navigationBar: navigationBar

    Rectangle {
        id: background

        anchors.fill: parent

        gradient: Gradient {
            GradientStop {
                color: Colors.color.background0
                position: 0.0
            }

            GradientStop {
                color: Colors.color.background1
                position: 1.0
            }
        }

        RowLayout {
            anchors.fill: parent

            NavigationBar {
                id: navigationBar

                Layout.fillHeight: true
            }
        }
    }
}
