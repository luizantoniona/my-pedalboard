pragma Singleton

import QtQuick

QtObject {
    readonly property string family: "Segoe UI"
    readonly property string familyMono: "Consolas"

    readonly property int sizeCaption: 11
    readonly property int sizeBody: 13
    readonly property int sizeSubTitle: 16
    readonly property int sizeTitle: 20
    readonly property int sizeDisplay: 48

    readonly property int weightRegular: Font.Normal
    readonly property int weightMedium: Font.Medium
    readonly property int weightBold: Font.Bold
}
