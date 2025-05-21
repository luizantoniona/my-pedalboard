import QtQuick 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Item {
    id: root

    height: vSize
    width: vSize

    ColumnLayout {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        anchors.fill: parent
        spacing: 6

        // Knob Area
        Item {
            id: knobArea

            height: vSize
            width: vSize

            // Background ring (dark gray)
            Shape {
                anchors.centerIn: parent
                height: parent.height
                width: parent.width

                ShapePath {
                    capStyle: ShapePath.RoundCap
                    fillColor: "transparent"
                    strokeColor: "#2e3b40"
                    strokeWidth: 6

                    PathArc {
                        radiusX: width / 2
                        radiusY: height / 2
                        useLargeArc: true
                        x: width
                        y: height
                    }
                }

                ShapePath {
                    capStyle: ShapePath.RoundCap
                    fillColor: "transparent"
                    strokeColor: "#ff9933"
                    // Orange arc (value indicator)
                    strokeWidth: 6

                    PathArc {
                        radiusX: width / 2
                        radiusY: height / 2
                        useLargeArc: false
                        x: width
                        y: height
                    }
                }
            }

            // Main knob face
            Rectangle {
                id: knobFace

                anchors.centerIn: parent
                color: "#3c454a"
                height: width
                radius: width / 2
                width: parent.width * 0.8

                // Glossy effect (optional)
                Rectangle {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    height: parent.height * 0.5
                    opacity: 0.3
                    radius: parent.radius
                    width: parent.width

                    gradient: Gradient {
                        GradientStop {
                            color: "#5a666b"
                            position: 0.0
                        }

                        GradientStop {
                            color: "transparent"
                            position: 1.0
                        }
                    }
                }
            }

            // Pointer (orange vertical line)
            Rectangle {
                anchors.horizontalCenter: knobFace.horizontalCenter
                anchors.verticalCenter: knobFace.verticalCenter
                color: "#ff9933"
                height: knobFace.height / 2.2
                radius: 2
                width: 4

                transform: Rotation {
                    angle: 0 // placeholder for value rotation
                    origin.x: 2
                    origin.y: knobFace.height / 2.2
                }
            }
        }

        // Label
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            color: vLabelColor
            font.pixelSize: 14
            horizontalAlignment: Text.AlignHCenter
            text: vLabel
        }
    }
}
