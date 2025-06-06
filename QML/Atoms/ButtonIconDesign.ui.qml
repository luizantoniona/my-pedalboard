import QtQuick
import QtQuick.Controls

Button {
    id: root

    visible: vIconName !== ""
    width: vIconSize
    height: vIconSize
    icon.source: "qrc:/icons/outlined/" + vIconName + ".svg"
    icon.color: "blue"
}
