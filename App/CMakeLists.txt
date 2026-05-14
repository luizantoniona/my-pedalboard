qt_add_executable(MyPedalboard
    main.cpp
)

qt_add_qml_module(MyPedalboard
    URI main
    VERSION 1.0
    QML_FILES
        Main.qml
)

target_link_libraries(MyPedalboard
    PRIVATE ControlLib
    PRIVATE Qt6::QuickControls2
    PRIVATE ${QML_MODULES}
)
