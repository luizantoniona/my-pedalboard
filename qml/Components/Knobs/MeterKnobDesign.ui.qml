import QtQuick 2.15
import Components 1.0

Item {
    id: root

    height: vSize
    width: vSize

    ArcMeter {
        id: knobArc

        vSize: root.vSize
        vIntensity: knob.vIntensity
        anchors.centerIn: parent
    }

    BasicKnob {
        id: knob

        anchors.centerIn: parent
        vSize: root.vSize - knobArc.outsidePath.strokeWidth
    }
}
