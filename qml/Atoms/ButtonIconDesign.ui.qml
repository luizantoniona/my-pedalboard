import QtQuick
import QtQuick.Controls
import Atoms

Button {
    id: root

    width: 40
    height: 40
    background: Rectangle {
        color: root.down ? "#444" : "#333"
        radius: 8
        border.color: "#555"
    }

    contentItem: Image {
        anchors.centerIn: parent
        source: "qrc:/icons/" + iconName + ".svg"
        width: root.iconSize
        height: root.iconSize
    }
}
