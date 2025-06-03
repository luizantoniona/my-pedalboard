pragma Singleton

import QtQuick

QtObject {
    property bool darkMode: true
    readonly property var colors: darkMode ? ColorsDark : ColorsLight
}
