import QtQuick
import QtQuick.Layouts
import MyPedalboardGUIComponents

ColumnLayout {
    id: root

    property bool vVoiced: false
    property bool vInTune: false
    property string vNote: "-"
    property int vOctave: 0
    property real vCents: 0.0
    property real vFrequency: 0.0

    readonly property real _clampedCents: Math.max(-50, Math.min(50, root.vCents))

    spacing: Spaces.spacing2

    Text {
        Layout.alignment: Qt.AlignHCenter

        text: root.vVoiced ? root.vNote + root.vOctave : "-"
        color: !root.vVoiced
               ? Colors.textTertiary
               : (root.vInTune ? Colors.success : Colors.textPrimary)
        font.family: Fonts.family
        font.pixelSize: Fonts.sizeDisplay
        font.weight: Fonts.weightBold
    }

    Item {
        Layout.fillWidth: true
        Layout.preferredHeight: 34

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right

            height: 4
            radius: Radiuses.radius4
            color: Colors.background1
        }

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            width: 2
            height: 22
            color: Colors.border
        }

        Rectangle {
            id: needle

            anchors.verticalCenter: parent.verticalCenter

            x: parent.width / 2 + (root._clampedCents / 50) * (parent.width / 2 - width) - width / 2
            width: 6
            height: 30
            radius: Radiuses.radius4
            visible: root.vVoiced
            color: root.vInTune ? Colors.success : Colors.primary

            Behavior on x {
                NumberAnimation {
                    duration: 80
                }
            }
        }
    }

    Text {
        Layout.alignment: Qt.AlignHCenter

        text: root.vVoiced
              ? root.vFrequency.toFixed(1) + " Hz   " + (root.vCents >= 0 ? "+" : "") + root.vCents.toFixed(0) + " cents"
              : "toque uma corda"
        color: Colors.textSecondary
        font.family: Fonts.familyMono
        font.pixelSize: Fonts.sizeCaption
    }
}
