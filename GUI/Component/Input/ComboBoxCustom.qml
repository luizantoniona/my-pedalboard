import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MyPedalboardGUIComponents

ColumnLayout {
    id: root

    signal activated(int index)

    property alias vModel: combo.model
    property alias vCurrentIndex: combo.currentIndex
    property string vLabel: ""

    spacing: Spaces.spacing1

    Text {
        Layout.fillWidth: true

        text: root.vLabel
        visible: root.vLabel !== ""
        color: Colors.textSecondary
        font.family: Fonts.family
        font.pixelSize: Fonts.sizeCaption
    }

    ComboBox {
        id: combo

        Layout.fillWidth: true
        Layout.preferredHeight: 36

        background: Rectangle {
            color: Colors.background3
            radius: Radiuses.radius4
            border.color: Colors.border
            border.width: Borders.thin
        }

        contentItem: Text {
            leftPadding: Spaces.spacing2
            rightPadding: Spaces.spacing5

            text: combo.displayText
            color: Colors.textPrimary
            font.family: Fonts.family
            font.pixelSize: Fonts.sizeBody
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }

        onActivated: function (index) {
            root.activated(index)
        }
    }
}
