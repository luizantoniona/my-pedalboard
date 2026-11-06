import QtQuick
import QtQuick.Layouts
import MyPedalboardGUIComponents

ColumnLayout {
    id: root

    property string vLabel: ""
    property real vLevel: 0.0
    property bool vClipping: false

    spacing: Spaces.spacing1

    Text {
        Layout.fillWidth: true

        text: root.vLabel
        visible: root.vLabel !== ""
        color: Colors.textSecondary
        font.family: Fonts.family
        font.pixelSize: Fonts.sizeCaption
    }

    Rectangle {
        Layout.fillWidth: true
        Layout.preferredHeight: 10

        color: Colors.background1
        radius: Radiuses.radius4
        border.color: root.vClipping ? Colors.error : Colors.border
        border.width: Borders.thin
        clip: true

        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: Borders.thin

            width: Math.max(0, Math.min(1, root.vLevel)) * (parent.width - Borders.thin * 2)
            radius: Radiuses.radius4

            color: root.vClipping
                   ? Colors.error
                   : (root.vLevel > 0.85 ? Colors.warning : Colors.success)

            Behavior on width {
                NumberAnimation {
                    duration: 40
                }
            }
        }
    }
}
