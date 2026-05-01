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
        anchors.leftMargin: Spacing.spacing4
        anchors.rightMargin: Spacing.spacing4
        spacing: Spacing.spacing4

        ComboBoxCustom {
            model: audioControl.inputDevices

            onCurrentIndexChanged: function () {
                audioControl.setInputDevice(this.currentIndex)
            }
        }

        Item {
            Layout.fillWidth: true
        }

        ButtonText {
            vText: "PLAY"
            onClicked: function () {
                audioControl.start()
            }
        }

        ButtonText {
            vText: "STOP"
            onClicked: function () {
                audioControl.stop()
            }
        }

        Item {
            Layout.fillWidth: true
        }

        ComboBoxCustom {
            model: audioControl.outputDevices

            onCurrentIndexChanged: function () {
                audioControl.setOutputDevice(this.currentIndex)
            }
        }
    }
}
