import QtQuick
import QtQuick.Controls

Button {
    id: root

    height: vIconSize
    icon.color: "blue"
    icon.source: "qrc:/icons/" + vIconName + ".svg"
    visible: vIconName !== ""
    width: vIconSize
}
