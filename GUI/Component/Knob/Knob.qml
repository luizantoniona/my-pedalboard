import QtQuick
import MyPedalboardGUIComponents

Item {
    id: root

    signal moved(real value)

    property string vLabel: ""
    property string vUnit: ""
    property real vMinimum: 0.0
    property real vMaximum: 1.0
    property real vDefaultValue: 0.0
    property real vValue: 0.0
    property real vSize: 52
    property color vColorPointer: Colors.primary

    readonly property real _span: Math.max(0.000001, vMaximum - vMinimum)
    readonly property real _position: Math.max(0, Math.min(1, (vValue - vMinimum) / _span))

    // Leaves a gap at the bottom so the ends of the travel are distinguishable
    readonly property real _startAngle: -135
    readonly property real _sweepAngle: 270

    implicitWidth: vSize
    implicitHeight: vSize + Spaces.spacing5

    function _apply(value) {
        var clamped = Math.max(root.vMinimum, Math.min(root.vMaximum, value));
        if (clamped === root.vValue) {
            return;
        }
        root.vValue = clamped;
        root.moved(clamped);
    }

    Column {
        anchors.fill: parent

        spacing: Spaces.spacing1

        Item {
            width: root.vSize
            height: root.vSize

            Rectangle {
                anchors.fill: parent

                radius: width / 2
                color: Colors.background3
                border.color: dragArea.pressed ? Colors.borderFocus : Colors.border
                border.width: Borders.medium
            }

            Rectangle {
                id: pointer

                readonly property real angle: root._startAngle + root._sweepAngle * root._position

                width: Borders.thick
                height: root.vSize * 0.34
                radius: Radiuses.radius4
                color: root.vColorPointer
                antialiasing: true

                x: (parent.width - width) / 2
                y: root.vSize * 0.1

                transform: Rotation {
                    origin.x: pointer.width / 2
                    origin.y: root.vSize / 2 - pointer.y

                    angle: pointer.angle
                }
            }

            MouseArea {
                id: dragArea

                property real originY: 0
                property real originValue: 0

                anchors.fill: parent

                acceptedButtons: Qt.LeftButton

                onPressed: function (mouse) {
                    originY = mouse.y;
                    originValue = root.vValue;
                }

                onPositionChanged: function (mouse) {
                    if (!pressed) {
                        return;
                    }
                    var sensitivity = (mouse.modifiers & Qt.ShiftModifier) ? 600 : 150;
                    root._apply(originValue + ((originY - mouse.y) / sensitivity) * root._span);
                }

                onDoubleClicked: root._apply(root.vDefaultValue)
            }
        }

        Text {
            width: root.vSize

            text: root.vLabel
            color: Colors.textSecondary
            font.family: Fonts.family
            font.pixelSize: Fonts.sizeCaption
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
        }

        Text {
            width: root.vSize

            text: root.vValue.toFixed(root._span > 10 ? 0 : 2) + (root.vUnit !== "" ? " " + root.vUnit : "")
            color: Colors.textPrimary
            font.family: Fonts.familyMono
            font.pixelSize: Fonts.sizeCaption
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
        }
    }
}
