import QtQuick
import QtQuick.Layouts
import MyPedalboardGUIComponents

Item {
    id: root

    Rectangle {
        anchors.fill: parent

        color: Colors.background1
    }

    ColumnLayout {
        anchors.centerIn: parent

        spacing: Spaces.spacing2

        Text {
            Layout.alignment: Qt.AlignHCenter

            text: "My Pedalboard"
            color: Colors.textPrimary
            font.family: Fonts.family
            font.pixelSize: Fonts.sizeTitle
            font.weight: Fonts.weightBold
        }

        Text {
            Layout.alignment: Qt.AlignHCenter

            text: "Processador de efeitos de guitarra em tempo real"
            color: Colors.textSecondary
            font.family: Fonts.family
            font.pixelSize: Fonts.sizeBody
        }
    }
}
