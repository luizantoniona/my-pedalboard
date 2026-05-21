qt_add_executable(MyPedalboard
    main.cpp
)

qt_add_qml_module(MyPedalboard
    URI main
    VERSION 1.0
    QML_FILES
        Main.qml

add_subdirectory(Control)

file(GLOB_RECURSE SVG_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Assets/*.svg
)

qt_add_resources(${PROJECT_NAME}
    PREFIX "/Assets"
    BASE "Assets"
    FILES ${SVG_FILES}
)

target_link_libraries(MyPedalboard
    PRIVATE ControlLib
    PRIVATE Qt6::QuickControls2
    PRIVATE ${QML_MODULES}
)
