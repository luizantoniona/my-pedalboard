import QtQuick
import QtQuick.Controls
import MyPedalboardGUIComponents

Item {
    id: root

    signal clicked

    property color vColorBackground: Colors.background2
    property color vColorBorder: Colors.primary
    property color vColorIcon: Colors.primary

    property string vIcon: ""
    property real vWidth: 40
    property real vHeight: 40

    width: vWidth
    height: vHeight
    visible: vIcon !== ""

    Button {
        anchors.fill: parent

        background: Rectangle {
            color: root.vColorBackground
            radius: Radiuses.radius4
            border.color: root.vColorBorder
            border.width: Borders.thin
        }

        icon {
            source: root.vIcon
            color: root.vColorIcon
            height: root.vHeight * 0.55
            width: root.vWidth * 0.55
        }

        onClicked: root.clicked()
    }
}
