import QtQuick
import QtQuick.Shapes

Item {
    id: root

    property alias insidePath: insidePath
    property alias outsidePath: outsidePath

    height: vSize
    width: vSize

    Shape {
        anchors.centerIn: parent
        height: parent.height
        width: parent.width

        ShapePath {
            id: outsidePath

            capStyle: ShapePath.RoundCap
            fillColor: Colors.backgroundT
            strokeColor: vOutlineColor
            strokeWidth: vSize / 16

            PathAngleArc {
                centerX: vSize / 2
                centerY: vSize / 2
                radiusX: (vSize - outsidePath.strokeWidth) / 2
                radiusY: (vSize - outsidePath.strokeWidth) / 2
                startAngle: vStartAngle
                sweepAngle: vMaxAngle
            }
        }

        ShapePath {
            id: insidePath

            capStyle: ShapePath.RoundCap
            fillColor: Colors.backgroundT
            strokeColor: vFillColor
            strokeWidth: vSize / 24

            PathAngleArc {
                centerX: vSize / 2
                centerY: vSize / 2
                radiusX: (vSize - outsidePath.strokeWidth) / 2
                radiusY: (vSize - outsidePath.strokeWidth) / 2
                startAngle: vStartAngle
                sweepAngle: vIntensityInternal * vMaxAngle
            }
        }
    }
}
