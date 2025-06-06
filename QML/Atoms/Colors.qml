pragma Singleton

import QtQuick
import Atoms

QtObject {
    property bool darkMode: true

    readonly property var colors: darkMode ? ColorsDark : ColorsLight
}
