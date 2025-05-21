import QtQuick 2.15

KnobDesign {
    id: root

    property string vLabel: "Vol"
    property color vLabelColor: "#FFFFFF"

    //    knobArea.onReleased: {
    //        click = click + 1;
    //        console.info("Pressionado:" + click);
    //    }
    property int vSize: 80
    property real value: 0.75
}
