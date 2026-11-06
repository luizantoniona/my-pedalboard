import QtQuick
import QtQuick.Layouts
import MyPedalboardGUIComponents

RowLayout {
    id: root

    property bool vVoiced: false
    property bool vInTune: false
    property string vNote: "-"
    property int vOctave: 0
    property real vCents: 0.0
    property real vFrequency: 0.0

    readonly property real _clampedCents: Math.max(-50, Math.min(50, root.vCents))

    spacing: Spaces.spacing3

    Text {
        Layout.preferredWidth: 48

        text: root.vVoiced ? root.vNote + root.vOctave : "-"
        color: !root.vVoiced
               ? Colors.textTertiary
               : (root.vInTune ? Colors.success : Colors.textPrimary)
        font.family: Fonts.family
        font.pixelSize: Fonts.sizeTitle
        font.weight: Fonts.weightBold
        horizontalAlignment: Text.AlignHCenter
    }

    Item {
        Layout.fillWidth: true
        Layout.preferredHeight: 20

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.right: parent.right

            height: 2
            radius: Radiuses.radius4
            color: Colors.background4
        }

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            width: 2
            height: 14
            color: Colors.textTertiary
        }

        Rectangle {
            id: needle

            anchors.verticalCenter: parent.verticalCenter

            x: parent.width / 2 + (root._clampedCents / 50) * (parent.width / 2 - width) - width / 2
            width: 4
            height: 18
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
        Layout.preferredWidth: 120

        text: root.vVoiced
              ? root.vFrequency.toFixed(1) + " Hz  " + (root.vCents >= 0 ? "+" : "") + root.vCents.toFixed(0) + "¢"
              : "afinador"
        color: Colors.textTertiary
        font.family: Fonts.familyMono
        font.pixelSize: Fonts.sizeCaption
        horizontalAlignment: Text.AlignRight
    }
}
