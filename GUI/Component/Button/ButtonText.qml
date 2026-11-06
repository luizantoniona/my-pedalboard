import QtQuick
import QtQuick.Controls
import MyPedalboardGUIComponents

Item {
    id: root

    signal clicked

    property alias vText: label.text
    property color vColorBackground: Colors.background2
    property color vColorBorder: Colors.primary
    property color vColorText: Colors.textPrimary

    property real vWidth: 120
    property real vHeight: 40

    implicitWidth: vWidth
    implicitHeight: vHeight

    Button {
        anchors.fill: parent

        background: Rectangle {
            color: root.vColorBackground
            radius: Radiuses.radius4
            border.color: root.vColorBorder
            border.width: Borders.thin
        }

        contentItem: Text {
            id: label

            color: root.vColorText
            font.family: Fonts.family
            font.pixelSize: Fonts.sizeBody
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: root.clicked()
    }
}
