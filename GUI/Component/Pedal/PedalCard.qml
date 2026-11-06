import QtQuick
import QtQuick.Layouts
import MyPedalboardGUIComponents

Rectangle {
    id: root

    signal toggled(bool enabled)
    signal parameterMoved(string name, real value)
    signal moveLeftRequested
    signal moveRightRequested
    signal removeRequested

    property string vTypeName: ""
    property string vDisplayName: ""
    property bool vEnabled: true
    property var vParameters: []
    property bool vCanMoveLeft: false
    property bool vCanMoveRight: false

    // Called with a parameter name; returns the value currently held by the DSP
    property var vValueOf: null

    // Many parameters read better as a row of faders than as a wall of knobs
    readonly property bool _useFaders: vParameters.length > 6

    readonly property color _accent: {
        switch (vTypeName) {
        case "Overdrive":
            return Colors.pedalYellow;
        case "Equalizer":
            return Colors.pedalBlue;
        case "HighGain":
            return Colors.pedalOrange;
        case "Chorus":
            return Colors.pedalCyan;
        case "Delay":
            return Colors.pedalGreen;
        case "Reverb":
            return Colors.pedalPurple;
        case "NoiseGate":
            return Colors.pedalRed;
        default:
            return Colors.pedalGray;
        }
    }

    implicitWidth: layout.implicitWidth + Spaces.spacing4 * 2
    implicitHeight: layout.implicitHeight + Spaces.spacing4 * 2

    radius: Radiuses.radius8
    border.color: Colors.pedalPanel
    border.width: Borders.medium

    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: root.vEnabled ? Qt.lighter(root._accent, 1.15) : Qt.darker(root._accent, 2.4)
        }

        GradientStop {
            position: 1.0
            color: root.vEnabled ? Qt.darker(root._accent, 1.25) : Qt.darker(root._accent, 3.2)
        }
    }

    ColumnLayout {
        id: layout

        anchors.fill: parent
        anchors.margins: Spaces.spacing3

        spacing: Spaces.spacing3

        RowLayout {
            Layout.fillWidth: true

            spacing: Spaces.spacing1

            Rectangle {
                Layout.preferredWidth: 10
                Layout.preferredHeight: 10

                radius: width / 2
                color: root.vEnabled ? Colors.ledOn : Colors.ledOff
                border.color: Colors.pedalPanel
                border.width: Borders.thin
            }

            Item {
                Layout.fillWidth: true
            }

            ButtonText {
                vText: "◀"
                vWidth: 24
                vHeight: 24
                vColorBackground: Colors.pedalPanel
                vColorBorder: root.vCanMoveLeft ? Colors.pedalMetalDark : Colors.pedalPanel
                vColorText: root.vCanMoveLeft ? Colors.textPrimary : Colors.textDisabled
                enabled: root.vCanMoveLeft

                onClicked: root.moveLeftRequested()
            }

            ButtonText {
                vText: "▶"
                vWidth: 24
                vHeight: 24
                vColorBackground: Colors.pedalPanel
                vColorBorder: root.vCanMoveRight ? Colors.pedalMetalDark : Colors.pedalPanel
                vColorText: root.vCanMoveRight ? Colors.textPrimary : Colors.textDisabled
                enabled: root.vCanMoveRight

                onClicked: root.moveRightRequested()
            }

            ButtonIcon {
                vIcon: Icons.outlinedTrash
                vWidth: 24
                vHeight: 24
                vColorBackground: Colors.pedalPanel
                vColorBorder: Colors.pedalPanel
                vColorIcon: Colors.error

                onClicked: root.removeRequested()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: panel.implicitHeight + Spaces.spacing3 * 2
            Layout.preferredWidth: panel.implicitWidth + Spaces.spacing3 * 2

            radius: Radiuses.radius4
            color: Colors.pedalPanel
            border.color: Colors.background4
            border.width: Borders.thin

            RowLayout {
                id: panel

                anchors.centerIn: parent

                spacing: root._useFaders ? Spaces.spacing2 : Spaces.spacing3

                Repeater {
                    model: root._useFaders ? [] : root.vParameters

                    Knob {
                        required property var modelData

                        vLabel: modelData.label
                        vUnit: modelData.unit
                        vMinimum: modelData.minimum
                        vMaximum: modelData.maximum
                        vDefaultValue: modelData.defaultValue
                        vColorPointer: root._accent
                        vValue: root.vValueOf ? root.vValueOf(modelData.name) : modelData.defaultValue

                        onMoved: function (value) {
                            root.parameterMoved(modelData.name, value);
                        }
                    }
                }

                Repeater {
                    model: root._useFaders ? root.vParameters : []

                    Fader {
                        required property var modelData

                        vLabel: modelData.label
                        vMinimum: modelData.minimum
                        vMaximum: modelData.maximum
                        vDefaultValue: modelData.defaultValue
                        vColorFill: root._accent
                        vValue: root.vValueOf ? root.vValueOf(modelData.name) : modelData.defaultValue

                        onMoved: function (value) {
                            root.parameterMoved(modelData.name, value);
                        }
                    }
                }
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter

            text: (root.vDisplayName !== "" ? root.vDisplayName : root.vTypeName).toUpperCase()
            color: Colors.pedalPanelText
            font.family: Fonts.family
            font.pixelSize: Fonts.sizeSubTitle
            font.weight: Fonts.weightBold
            font.letterSpacing: 2
            opacity: root.vEnabled ? 1.0 : 0.5
        }

        Item {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 56
            Layout.preferredHeight: 56

            Rectangle {
                id: footswitch

                anchors.fill: parent

                radius: width / 2
                scale: footswitchArea.pressed ? 0.94 : 1.0
                border.color: Colors.pedalPanel
                border.width: Borders.medium

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

                Rectangle {
                    anchors.centerIn: parent

                    width: parent.width * 0.55
                    height: width
                    radius: width / 2
                    color: Colors.pedalMetalDark
                    border.color: Colors.pedalMetalLight
                    border.width: Borders.thin
                }

                Behavior on scale {
                    NumberAnimation {
                        duration: 60
                    }
                }
            }

            MouseArea {
                id: footswitchArea

                anchors.fill: parent

                onClicked: root.toggled(!root.vEnabled)
            }
        }
    }
}
