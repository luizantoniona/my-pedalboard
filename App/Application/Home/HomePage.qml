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

        spacing: Spaces.spacing5

        Rectangle {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 420
            Layout.preferredHeight: 200

            color: Colors.background2
            radius: Radiuses.radius8
            border.color: Colors.border
            border.width: Borders.thin

            TunerDial {
                anchors.fill: parent
                anchors.margins: Spaces.spacing4

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

        Item {
            Layout.fillHeight: true
        }
    }
}
