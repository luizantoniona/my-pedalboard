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
                position: 0.9
            }
        }

        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            NavigationBar {
                id: navigationBar

                Layout.alignment: Qt.AlignTop
                Layout.preferredHeight: background.height
            }
        }
    }
}
