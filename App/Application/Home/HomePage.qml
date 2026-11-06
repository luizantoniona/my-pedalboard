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
        anchors.fill: parent
        anchors.margins: Spaces.spacing5

        spacing: Spaces.spacing4

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 40

            color: Colors.background2
            radius: Radiuses.radius8

            TunerDial {
                anchors.fill: parent
                anchors.leftMargin: Spaces.spacing3
                anchors.rightMargin: Spaces.spacing3

                vVoiced: AppContext.tuner.voiced
                vInTune: AppContext.tuner.inTune
                vNote: AppContext.tuner.noteName
                vOctave: AppContext.tuner.octave
                vCents: AppContext.tuner.cents
                vFrequency: AppContext.tuner.frequency
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter

            text: AppContext.home.running
                  ? "Passthrough ativo"
                  : "Selecione os dispositivos em Configuracoes e pressione play"
            color: Colors.textSecondary
            font.family: Fonts.family
            font.pixelSize: Fonts.sizeBody
        }

        RowLayout {
            Layout.fillWidth: true

            spacing: Spaces.spacing3

            Text {
                text: "Cadeia"
                color: Colors.textPrimary
                font.family: Fonts.family
                font.pixelSize: Fonts.sizeSubTitle
                font.weight: Fonts.weightMedium
            }

            Item {
                Layout.fillWidth: true
            }

            ComboBoxCustom {
                id: typePicker

                // A nested layout defaults to fillWidth, which would eat the row
                Layout.fillWidth: false
                Layout.preferredWidth: 180

                vModel: AppContext.chain.availableNames
            }

            ButtonIcon {
                vIcon: Icons.outlinedPlus

                onClicked: {
                    if (typePicker.vCurrentIndex >= 0) {
                        AppContext.chain.addEffect(AppContext.chain.availableTypes[typePicker.vCurrentIndex]);
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            // A PedalCard sizes to its content; below this the ListView clips it
            Layout.minimumHeight: 380

            color: Colors.background2
            radius: Radiuses.radius8
            border.color: Colors.border
            border.width: Borders.thin

            Text {
                anchors.centerIn: parent

                text: "Nenhum pedal na cadeia"
                visible: AppContext.chain.count === 0
                color: Colors.textTertiary
                font.family: Fonts.family
                font.pixelSize: Fonts.sizeBody
            }

            ListView {
                id: chainView

                anchors.fill: parent
                anchors.margins: Spaces.spacing4

                orientation: ListView.Horizontal
                spacing: Spaces.spacing4
                clip: true
                model: AppContext.chain

                delegate: PedalCard {
                    required property int index
                    required property string typeName
                    required property string displayName
                    required property bool effectEnabled
                    required property var parameters

                    vTypeName: typeName
                    vDisplayName: displayName
                    vEnabled: effectEnabled
                    vParameters: parameters
                    vCanMoveLeft: index > 0
                    vCanMoveRight: index < AppContext.chain.count - 1
                    vValueOf: name => AppContext.chain.parameter(index, name)

                    onToggled: function (value) {
                        AppContext.chain.setEffectEnabled(index, value);
                    }

                    onParameterMoved: function (name, value) {
                        AppContext.chain.setParameter(index, name, value);
                    }

                    onMoveLeftRequested: AppContext.chain.moveEffect(index, index - 1)
                    onMoveRightRequested: AppContext.chain.moveEffect(index, index + 1)
                    onRemoveRequested: AppContext.chain.removeEffect(index)
                }
            }
        }
    }
}
