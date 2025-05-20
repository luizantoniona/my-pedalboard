pragma Singleton
import QtQuick 2.15

QtObject {
    readonly property string fontFamily: "Arial"

    readonly property font knobLabelFont: Qt.font({
        "family": fontFamily,
        "normal": true,
        "pixelSize": 12
    })
}
