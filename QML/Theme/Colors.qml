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
    readonly property color primary: "#FF6B35"        // Orange accent
    readonly property color primaryHover: "#FF8555"
    readonly property color primaryPressed: "#E55A25"
    readonly property color primaryMuted: "#CC5528"

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
    // LED Colors
    // ============================================
    readonly property color ledOn: "#FF6B35"
    readonly property color ledOff: "#404040"
    readonly property color ledBypass: "#F44336"

    // ============================================
    // Border Colors
    // ============================================
    readonly property color border: "#404040"
    readonly property color borderFocus: "#FF6B35"

    // ============================================
    // Knob Colors
    // ============================================
    readonly property color knobBackground: "#2A2A2A"
    readonly property color knobForeground: "#FF6B35"
    readonly property color knobIndicator: "#FFFFFF"
    readonly property color knobTrack: "#404040"
}