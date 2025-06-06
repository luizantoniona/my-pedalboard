pragma Singleton
import QtQuick
import Atoms

QtObject {
    readonly property var color: darkMode ? ColorsDark : ColorsLight
    property bool darkMode: true
}
