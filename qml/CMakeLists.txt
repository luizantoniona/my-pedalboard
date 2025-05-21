add_subdirectory(Colors)
add_subdirectory(Components)
add_subdirectory(Fonts)
add_subdirectory(Radiuses)
add_subdirectory(Screens)

file(GLOB_RECURSE QML_FILE_LIST
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.qml"
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/qml)

configure_file(main.qml ${CMAKE_BINARY_DIR}/qml/main.qml COPYONLY)
