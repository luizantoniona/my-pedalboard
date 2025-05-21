set(MODULE_NAME Screens)

file(GLOB_RECURSE QML_FILE_LIST
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.qml"
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeQt6QmlModule.cmake)
else()
    include(${CMAKE_SOURCE_DIR}/cmake/CMakeQt5QmlModule.cmake)
endif()
