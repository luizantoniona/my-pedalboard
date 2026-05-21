import QtQuick
import QtQuick.Layouts
import Theme

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: Colors.background1
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: Spacing.spacing0

        Item {
            Layout.fillHeight: true
        }

        Text {
            text: "VERSION: EARLY ACCESS"
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
