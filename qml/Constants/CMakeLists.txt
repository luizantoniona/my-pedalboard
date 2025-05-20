set(MODULE_NAME Constants)

set(QML_FILE_LIST
    Fonts.qml
    Colors.qml
)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/Fonts.qml
    ${CMAKE_CURRENT_SOURCE_DIR}/Colors.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeQt6QmlModule.cmake)

else()
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeQt5QmlModule.cmake)
endif()
