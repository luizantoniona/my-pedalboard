add_subdirectory(Constants)
add_subdirectory(Components)
add_subdirectory(Screens)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/qml)

configure_file(main.qml ${CMAKE_BINARY_DIR}/qml/main.qml COPYONLY)
