set(MODULE_NAME Components)

set(QML_FILE_LIST
    Knob.qml
    KnobDesign.ui.qml
    Pedal.qml
    PedalDesign.ui.qml
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeQt6QmlModule.cmake)

else()
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeQt5QmlModule.cmake)
endif()
