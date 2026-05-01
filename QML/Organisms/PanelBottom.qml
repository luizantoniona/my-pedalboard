import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Atoms
import Theme

Item {
    id: root

    property var audioControl

    Rectangle {
        id: background

        anchors.fill: parent
        color: Colors.surface
        border.color: Colors.border
        border.width: 1
    }

    RowLayout {
        anchors.fill: parent
        spacing: Spacing.spacing4

        Item {
            Layout.fillWidth: true
        }

        ButtonText {
            vText: "PLAY"
            onClicked: function () {
                audioControl.start()
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
