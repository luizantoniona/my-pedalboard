import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Atoms
import Molecules
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
            Layout.fillWidth: true
            model: audioControl.inputDevices

            onCurrentIndexChanged: function () {
                audioControl.setInputDevice(this.currentIndex)
            }
        }

        Item {
            Layout.fillWidth: true
        }

        ComboBoxCustom {
            Layout.fillWidth: true
            model: audioControl.outputDevices

            onCurrentIndexChanged: function () {
                audioControl.setOutputDevice(this.currentIndex)
            }
        }
    }
}
