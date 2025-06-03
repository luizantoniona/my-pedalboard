pragma Singleton

import QtQuick
import Atoms

QtObject {
    property bool darkMode: true
    readonly property QtObject colors: darkMode ? ColorsDark : ColorsLight
}
