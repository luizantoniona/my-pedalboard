import QtQuick
import QtQuick.Layouts
import MyPedalboardApp
import MyPedalboardGUIComponents

Item {
    id: root

    Rectangle {
        anchors.fill: parent

        color: Colors.background1
    }

    ColumnLayout {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Spaces.spacing5

        spacing: Spaces.spacing4

        Text {
            text: "Audio"
            color: Colors.textPrimary
            font.family: Fonts.family
            font.pixelSize: Fonts.sizeSubTitle
            font.weight: Fonts.weightBold
        }

        GridLayout {
            Layout.fillWidth: true

            columns: 2
            columnSpacing: Spaces.spacing4
            rowSpacing: Spaces.spacing4

            ComboBoxCustom {
                Layout.fillWidth: true

                vLabel: "Driver"
                vModel: AppContext.settings.driverApis

                onActivated: function (index) {
                    AppContext.settings.setDriverApi(index)
                }
            }

            ComboBoxCustom {
                Layout.fillWidth: true

                vLabel: "Taxa de amostragem"
                vModel: AppContext.settings.sampleRates

                onActivated: function (index) {
                    AppContext.settings.setSampleRate(index)
                }
            }

            ComboBoxCustom {
                Layout.fillWidth: true

                vLabel: "Entrada"
                vModel: AppContext.settings.inputDevices

                onActivated: function (index) {
                    AppContext.settings.setInputDevice(index)
                }
            }

            ComboBoxCustom {
                Layout.fillWidth: true

                vLabel: "Saida"
                vModel: AppContext.settings.outputDevices

                onActivated: function (index) {
                    AppContext.settings.setOutputDevice(index)
                }
            }

            ComboBoxCustom {
                Layout.fillWidth: true

                vLabel: "Buffer (frames)"
                vModel: AppContext.settings.frameBuffers
                vCurrentIndex: 1

                onActivated: function (index) {
                    AppContext.settings.setFrameBuffer(index)
                }
            }

            Item {
                Layout.fillWidth: true
            }
        }

        Text {
            text: "Buffers menores reduzem a latencia e aumentam o risco de xrun."
            color: Colors.textTertiary
            font.family: Fonts.family
            font.pixelSize: Fonts.sizeCaption
        }
    }
}
