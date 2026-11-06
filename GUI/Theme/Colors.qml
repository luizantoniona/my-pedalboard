pragma Singleton

import QtQuick

QtObject {
    // ============================================
    // Background Colors
    // ============================================
    readonly property color background1: "#1A1A1A"
    readonly property color background2: "#242424"
    readonly property color background3: "#2E2E2E"
    readonly property color background4: "#383838"

    // ============================================
    // Surface Colors
    // ============================================
    readonly property color surface: "#2A2A2A"
    readonly property color surfaceHover: "#3A3A3A"
    readonly property color surfacePressed: "#1F1F1F"

    // ============================================
    // Primary / Accent Colors
    // ============================================
    readonly property color primary: "#FF6B35"
    readonly property color primaryHover: "#FF8555"
    readonly property color primaryPressed: "#E55A25"

    // ============================================
    // Text Colors
    // ============================================
    readonly property color textPrimary: "#FFFFFF"
    readonly property color textSecondary: "#B0B0B0"
    readonly property color textTertiary: "#707070"
    readonly property color textDisabled: "#505050"

    // ============================================
    // Semantic Colors
    // ============================================
    readonly property color success: "#4CAF50"
    readonly property color warning: "#FFC107"
    readonly property color error: "#F44336"
    readonly property color info: "#2196F3"

    // ============================================
    // Pedal Colors
    // ============================================
    readonly property color pedalYellow: "#F2C230"
    readonly property color pedalBlue: "#3E86C8"
    readonly property color pedalRed: "#C8392B"
    readonly property color pedalOrange: "#E8702A"
    readonly property color pedalCyan: "#39B6D6"
    readonly property color pedalGreen: "#4FA66A"
    readonly property color pedalPurple: "#8E63C7"
    readonly property color pedalGray: "#8A8F98"
    readonly property color pedalPanel: "#121212"
    readonly property color pedalPanelText: "#161616"
    readonly property color pedalMetalLight: "#D8D8D8"
    readonly property color pedalMetalDark: "#7A7A7A"
    readonly property color ledOn: "#FF3B30"
    readonly property color ledOff: "#4A1F1C"

    // ============================================
    // Border Colors
    // ============================================
    readonly property color border: "#404040"
    readonly property color borderFocus: "#FF6B35"
}
