import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: Colors.background1
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: Spacing.spacing0

        Item {
            Layout.fillHeight: true
        }

        ComboBox {
            model: audioControl.inputDevices

            onCurrentIndexChanged: function () {
                audioControl.setInputDevice(this.currentIndex)
            }
        }

        Item {
            Layout.fillHeight: true
        }

        ComboBox {
            model: audioControl.outputDevices

            onCurrentIndexChanged: function () {
                audioControl.setOutputDevice(this.currentIndex)
            }
        }

        Item {
            Layout.fillHeight: true
        }

        Button {
            id: playButton

            implicitWidth: 64
            implicitHeight: 64
            text: "PLAY"

            onClicked: function () {
                audioControl.start()
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
