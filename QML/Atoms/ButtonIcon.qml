import QtQuick
import QtQuick.Controls
import Theme

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
            color: vColorBackground
            radius: Radius.radius8
            border.color: vColorBorder
            border.width: 1
        }

        icon {
            source: vIcon
            color: vColorIcon
            height: vHeight
            width: vWidth
        }

        onClicked: function () {
            root.clicked()
        }
    }
}
