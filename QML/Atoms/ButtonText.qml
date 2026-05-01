import QtQuick
import QtQuick.Controls
import Theme

Item {
    id: root

    signal clicked

    property color vColorBackground: Colors.background2
    property color vColorBorder: Colors.primary
    property color vColorText: Colors.primary

    property string vText: ""
    property real vWidth: 120
    property real vHeight: 40

    width: vWidth
    height: vHeight
    visible: vText !== ""

    Button {
        anchors.fill: parent

        background: Rectangle {
            color: vColorBackground
            radius: Radius.radius8
            border.color: vColorBorder
            border.width: 1
        }

        contentItem: Text {
            text: vText
            color: vColorText
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        onClicked: function () {
            root.clicked()
        }
    }
}
