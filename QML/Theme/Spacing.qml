pragma Singleton

import QtQuick

QtObject {
    // ============================================
    // Base Spacing Units (8pt grid system)
    // ============================================
    readonly property double spacing0: 0
    readonly property double spacing1: 4
    readonly property double spacing2: 8
    readonly property double spacing3: 12
    readonly property double spacing4: 16
    readonly property double spacing5: 24
    readonly property double spacing6: 32
    readonly property double spacing7: 48
    readonly property double spacing8: 64

    // ============================================
    // Component-specific
    // ============================================
    readonly property double cornerRadiusSmall: 4
    readonly property double cornerRadiusMedium: 8
    readonly property double cornerRadiusLarge: 12

    // ============================================
    // Icon Sizes
    // ============================================
    readonly property double iconSizeSmall: 16
    readonly property double iconSizeMedium: 24
    readonly property double iconSizeLarge: 32

    // ============================================
    // Component Heights
    // ============================================
    readonly property double topBarHeight: 48
    readonly property double navigationBarHeight: 64
    readonly property double moduleCardWidth: 120
    readonly property double moduleCardHeight: 160
    readonly property double knobSize: 64
    readonly property double ledSize: 12
}