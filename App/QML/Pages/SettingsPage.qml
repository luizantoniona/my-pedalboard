import QtQuick
import QtQuick.Layouts
import Control
import Atoms
import Theme

Item {
    id: root

    required property AudioControl vAudioControl

    Rectangle {
        anchors.fill: parent
        color: Colors.background1
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Spacing.spacing4
        spacing: Spacing.spacing4

        Text {
            text: "API Drive"
            color: Colors.textPrimary
            font.pixelSize: Fonts.fontSizeMedium
            font.weight: Font.Bold
        }

        ComboBoxCustom {
            Layout.fillWidth: true
            model: vAudioControl.driverAPIs

            onCurrentIndexChanged: function () {
                vAudioControl.setDriverAPI(this.currentIndex)
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: Spacing.spacing4

            ColumnLayout {
                spacing: Spacing.spacing4

                Text {
                    text: "Input Device"
                    color: Colors.textPrimary
                    font.pixelSize: Fonts.fontSizeMedium
                    font.weight: Font.Bold
                }

                ComboBoxCustom {
                    Layout.fillWidth: true
                    model: vAudioControl.inputDevices

                    onCurrentIndexChanged: function () {
                        vAudioControl.setInputDevice(this.currentIndex)
                    }
                }
            }

            Item {
                Layout.fillWidth: true
            }

            ColumnLayout {
                spacing: Spacing.spacing4

                Text {
                    text: "Output Device"
                    color: Colors.textPrimary
                    font.pixelSize: Fonts.fontSizeMedium
                    font.weight: Font.Bold
                }

                ComboBoxCustom {
                    Layout.fillWidth: true
                    model: vAudioControl.outputDevices

                    onCurrentIndexChanged: function () {
                        vAudioControl.setOutputDevice(this.currentIndex)
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
