set(MODULE_NAME Fonts)

file(GLOB_RECURSE QML_FILE_LIST
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.qml"
)

set_source_files_properties(
    ${CMAKE_CURRENT_SOURCE_DIR}/Fonts.qml
    PROPERTIES QT_QML_SINGLETON_TYPE TRUE
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeQt6QmlModule.cmake)
else()
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeQt5QmlModule.cmake)
endif()
