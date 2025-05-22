import QtQuick 2.15
import Colors 1.0

ArcMeterDesign {
    id: root

    property real vIntensityInternal: 0.0
    property real vIntensity: 0.0
    property int vSize: 80
    property color vFillColor: Colors.accent
    property color vOutlineColor: Colors.background0
    property int vStartAngle: 135
    property int vMaxAngle: 270

    onVIntensityChanged: {
        vIntensityInternal = Math.max(0.0, Math.min(1.0, vIntensity));
    }

    Behavior on vIntensity {
        NumberAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
}
