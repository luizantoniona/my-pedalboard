import QtQuick 2.15
import Colors 1.0

BasicKnobDesign {
    id: root

    property real vIntensityInternal: 0.0
    property real vIntensity: 0.0
    property int vSize: 80
    property color vKnobColor: Colors.background8
    property color vNeedleColor: Colors.background4
    readonly property real vAngle: vStartAngle + (vMaxAngle * vIntensityInternal)
    property int vStartAngle: 225
    property int vMaxAngle: 270

    onVIntensityChanged: {
        vIntensityInternal = Math.max(0.0, Math.min(1.0, vIntensity));
    }

    knobArea.onReleased: {
        //TODO - Add a function to set the value correctly and send signal
        console.info("Released");
        vIntensity = vIntensity + 0.1;
    }

    transform: Rotation {
        id: rot
        origin.x: width / 2
        origin.y: height / 2
        angle: vAngle
    }

    Behavior on vIntensity {
        NumberAnimation {
            duration: 200
            easing.type: Easing.InOutQuad
        }
    }
}
