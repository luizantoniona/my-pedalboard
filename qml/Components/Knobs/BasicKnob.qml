import QtQuick

BasicKnobDesign {
    id: root

    readonly property real vAngle: vStartAngle + (vMaxAngle * vIntensityInternal)
    property real vIntensity: 0.0
    property real vIntensityInternal: 0.0
    property color vKnobColor: Colors.background8
    property int vMaxAngle: 270
    property color vNeedleColor: Colors.background4
    property double vSensitivity: 1.0
    property int vSize: 80
    property int vStartAngle: 225

    function angleFromCenter(x, y) {
        const centerX = width / 2;
        const centerY = height / 2;
        const dx = x - centerX;
        const dy = centerY - y;
        let angle = Math.atan2(dy, dx) * 180 / Math.PI;
        if (angle < 0)
            angle += 360;
        return angle;
    }

    transform: Rotation {
        id: rot

        angle: vAngle
        origin.x: width / 2
        origin.y: height / 2
    }
    Behavior on vIntensity  {
        NumberAnimation {
            duration: 100
            easing.type: Easing.InOutQuad
        }
    }

    knobArea.onPositionChanged: event => {
        if (!knobArea.dragging) {
            return;
        }
        const currentAngle = angleFromCenter(event.x, event.y);
        let delta = currentAngle - knobArea.lastAngle;
        if (delta > 180) {
            delta -= 360;
        }
        if (delta < -180) {
            delta += 360;
        }
        const sensitivity = vSensitivity / vMaxAngle;
        vIntensity = Math.max(0.0, Math.min(1.0, vIntensity - delta * sensitivity));
        knobArea.lastAngle = currentAngle;
    }
    knobArea.onPressed: event => {
        knobArea.dragging = true;
        knobArea.lastAngle = angleFromCenter(event.x, event.y);
    }
    knobArea.onReleased: {
        knobArea.dragging = false;
    }
    onVIntensityChanged: {
        vIntensityInternal = Math.max(0.0, Math.min(1.0, vIntensity));
    }
}
