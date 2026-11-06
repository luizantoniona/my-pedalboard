import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MyPedalboardApp
import MyPedalboardAppComponents
import MyPedalboardGUIComponents

Window {
    id: root

    width: 1100
    height: 680
    minimumWidth: 820
    minimumHeight: 560
    visible: true
    title: "My Pedalboard"
    color: Colors.background1

    ColumnLayout {
        anchors.fill: parent

        spacing: 0

        PanelTop {
            ButtonIcon {
                vIcon: Icons.outlinedHome
                vColorIcon: stack.currentIndex === 0 ? Colors.primary : Colors.textTertiary
                vColorBorder: stack.currentIndex === 0 ? Colors.primary : Colors.border

                onClicked: stack.currentIndex = 0
            }

            ButtonIcon {
                vIcon: Icons.outlinedSettings
                vColorIcon: stack.currentIndex === 1 ? Colors.primary : Colors.textTertiary
                vColorBorder: stack.currentIndex === 1 ? Colors.primary : Colors.border

                onClicked: stack.currentIndex = 1
            }

            ButtonIcon {
                vIcon: Icons.outlinedInfoCircle
                vColorIcon: stack.currentIndex === 2 ? Colors.primary : Colors.textTertiary
                vColorBorder: stack.currentIndex === 2 ? Colors.primary : Colors.border

                onClicked: stack.currentIndex = 2
            }

            Item {
                Layout.fillWidth: true
            }

            LevelBar {
                Layout.preferredWidth: 140

                vLabel: "entrada"
                vLevel: AppContext.levels.inputPeak
                vClipping: AppContext.levels.clipping
            }

            LevelBar {
                Layout.preferredWidth: 140

                vLabel: "saida"
                vLevel: AppContext.levels.outputPeak
            }

            Text {
                text: AppContext.home.latencyText
                color: AppContext.home.running ? Colors.success : Colors.textTertiary
                font.family: Fonts.familyMono
                font.pixelSize: Fonts.sizeBody
            }

            ButtonIcon {
                vIcon: AppContext.home.running ? Icons.outlinedPlayerStop : Icons.outlinedPlayerPlay
                vColorIcon: AppContext.home.running ? Colors.error : Colors.success
                vColorBorder: AppContext.home.running ? Colors.error : Colors.success

                onClicked: AppContext.home.toggle()
            }
        }

        StackLayout {
            id: stack

            Layout.fillWidth: true
            Layout.fillHeight: true

            currentIndex: 0

            HomePage {}

            SettingsPage {}

            AboutPage {}
        }

        PanelBottom {
            Text {
                text: AppContext.home.streamText
                color: Colors.textTertiary
                font.family: Fonts.familyMono
                font.pixelSize: Fonts.sizeCaption
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                text: AppContext.home.lastError
                visible: AppContext.home.lastError !== ""
                color: Colors.error
                font.family: Fonts.family
                font.pixelSize: Fonts.sizeCaption
            }
        }
    }
}
