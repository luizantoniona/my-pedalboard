import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic

Item {
    id: root

    property alias button: button

    height: vSize
    width: vSize
    visible: vIconName !== ""

    Button {
        id: button

        anchors.fill: parent
        background: Rectangle {
            color: vColorBackground
        }
        icon {
            source: "qrc:/icons/" + vIconName + ".svg"
            color: vColorIcon
            height: vSize
            width: vSize
        }
    }
}
