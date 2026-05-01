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
        anchors.leftMargin: Spacing.spacing4
        anchors.rightMargin: Spacing.spacing4
        spacing: Spacing.spacing4

        Item {
            Layout.fillHeight: true
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 240
            color: Colors.surface
            radius: 18
            border.color: Colors.border
            border.width: 1

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: Spacing.spacing4
                spacing: Spacing.spacing3

                Text {
                    text: "Audio pass through"
                    color: Colors.textPrimary
                    font.pixelSize: 22
                    font.weight: Font.DemiBold
                }

                Text {
                    text: "Select your input and output devices from the top bar, then tap Play below to route audio through the system."
                    wrapMode: Text.WordWrap
                    color: Colors.textSecondary
                    font.pixelSize: 15
                }

                Rectangle {
                    color: Colors.background2
                    radius: 12
                    border.color: Colors.border
                    border.width: 1
                    height: 110
                    width: parent.width

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: Spacing.spacing3
                        spacing: Spacing.spacing2

                        Text {
                            text: "Device selectors and controls are available in the top bar."
                            color: Colors.textSecondary
                            wrapMode: Text.WordWrap
                        }

                        Text {
                            text: "Play controls live in the bottom bar for easier access while monitoring."
                            color: Colors.textSecondary
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
