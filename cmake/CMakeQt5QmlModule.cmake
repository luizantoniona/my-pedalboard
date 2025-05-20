message(STATUS "Configuring: ${MODULE_NAME} With: Qt5QML")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/qml/${MODULE_NAME})

foreach(qml_file ${QML_FILE_LIST})
    configure_file(${qml_file} ${CMAKE_BINARY_DIR}/qml/${MODULE_NAME}/ COPYONLY)
endforeach()

configure_file(qmldir ${CMAKE_BINARY_DIR}/qml/${MODULE_NAME}/qmldir COPYONLY)
