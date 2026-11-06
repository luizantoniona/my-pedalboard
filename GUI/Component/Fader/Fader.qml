import QtQuick
import MyPedalboardGUIComponents

Item {
    id: root

    signal moved(real value)

    property string vLabel: ""
    property real vMinimum: -1.0
    property real vMaximum: 1.0
    property real vDefaultValue: 0.0
    property real vValue: 0.0
    property color vColorFill: Colors.primary
    property real vTravel: 120

    readonly property real _handleHeight: 14
    readonly property real _span: Math.max(0.000001, vMaximum - vMinimum)
    readonly property real _position: Math.max(0, Math.min(1, (vValue - vMinimum) / _span))
    readonly property real _centerPosition: Math.max(0, Math.min(1, (vDefaultValue - vMinimum) / _span))

    implicitWidth: 34
    implicitHeight: vTravel + Spaces.spacing5 * 2

    function _yOf(position) {
        return _handleHeight / 2 + (1 - position) * (vTravel - _handleHeight);
    }

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

        Text {
            width: root.width

            text: (root.vValue > 0.05 ? "+" : "") + root.vValue.toFixed(0)
            color: Colors.textPrimary
            font.family: Fonts.familyMono
            font.pixelSize: Fonts.sizeCaption
            horizontalAlignment: Text.AlignHCenter
        }

        Item {
            width: root.width
            height: root.vTravel

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter

                y: root._handleHeight / 2
                width: 4
                height: root.vTravel - root._handleHeight
                radius: Radiuses.radius4
                color: Colors.background3
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter

                y: Math.min(root._yOf(root._position), root._yOf(root._centerPosition))
                width: 4
                height: Math.abs(root._yOf(root._position) - root._yOf(root._centerPosition))
                color: root.vColorFill
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter

                y: root._yOf(root._centerPosition)
                width: 14
                height: Borders.thin
                color: Colors.textTertiary
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter

                y: root._yOf(root._position) - root._handleHeight / 2
                width: 26
                height: root._handleHeight
                radius: Radiuses.radius4
                border.color: dragArea.pressed ? Colors.borderFocus : Colors.pedalMetalDark
                border.width: Borders.thin

                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: Colors.pedalMetalLight
                    }

                    GradientStop {
                        position: 1.0
                        color: Colors.pedalMetalDark
                    }
                }
            }

            MouseArea {
                id: dragArea

                anchors.fill: parent

                acceptedButtons: Qt.LeftButton

                function _valueAt(y) {
                    var position = 1 - (y - root._handleHeight / 2) / (root.vTravel - root._handleHeight);
                    return root.vMinimum + Math.max(0, Math.min(1, position)) * root._span;
                }

                onPressed: function (mouse) {
                    root._apply(_valueAt(mouse.y));
                }

                onPositionChanged: function (mouse) {
                    if (pressed) {
                        root._apply(_valueAt(mouse.y));
                    }
                }

                onDoubleClicked: root._apply(root.vDefaultValue)
            }
        }

        Text {
            width: root.width

            text: root.vLabel
            color: Colors.textSecondary
            font.family: Fonts.family
            font.pixelSize: Fonts.sizeCaption
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
        }
    }
}
