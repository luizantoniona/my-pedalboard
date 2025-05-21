import QtQuick 2.15
import QtQuick.Shapes 1.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Item {
    id: root

    height: vSize
    width: vSize

    Rectangle {
        anchors.fill: parent
        color: "#ffd0e2"

        ColumnLayout {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            anchors.fill: parent

            Item {
                id: knobArea

                height: vSize
                width: vSize

                Shape {
                    anchors.centerIn: parent
                    height: parent.height
                    width: parent.width

                    ShapePath {
                        id: fillableShapePath

                        capStyle: ShapePath.RoundCap
                        fillColor: "transparent"
                        strokeColor: "#2e3b40"
                        strokeWidth: vSize / 8

                        PathAngleArc {
                            centerX: vSize / 2
                            centerY: vSize / 2
                            radiusX: (vSize - fillableShapePath.strokeWidth) / 2
                            radiusY: (vSize - fillableShapePath.strokeWidth) / 2
                            startAngle: 180
                            sweepAngle: 180
                        }
                    }

                    ShapePath {
                        id: valueShapePath

                        capStyle: ShapePath.RoundCap
                        fillColor: "transparent"
                        strokeColor: "#ff9933"
                        strokeWidth: vSize / 16

                        PathAngleArc {
                            centerX: vSize / 2
                            centerY: vSize / 2
                            radiusX: (vSize - fillableShapePath.strokeWidth) / 2
                            radiusY: (vSize - fillableShapePath.strokeWidth) / 2
                            startAngle: 180
                            sweepAngle: vIntensity * 180
                        }
                    }
                }
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                color: vLabelColor
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                text: vLabel
            }
        }
    }
}
