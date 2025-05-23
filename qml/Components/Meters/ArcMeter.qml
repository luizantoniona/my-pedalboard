import QtQuick 2.15
import Colors 1.0

ArcMeterDesign {
    id: root

    property color vFillColor: Colors.accent
    property real vIntensity: 0.0
    property real vIntensityInternal: 0.0
    property int vMaxAngle: 270
    property color vOutlineColor: Colors.background0
    property int vSize: 80
    property int vStartAngle: 135

    Behavior on vIntensity  {
        NumberAnimation {
            duration: 100
            easing.type: Easing.InOutQuad
        }
    }

    onVIntensityChanged: {
        vIntensityInternal = Math.max(0.0, Math.min(1.0, vIntensity));
    }
}
