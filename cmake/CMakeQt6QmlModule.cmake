message(STATUS "Configuring: ${MODULE_NAME} With: Qt6QML")

qt_add_qml_module(
    ${MODULE_NAME}
    URI ${MODULE_NAME}
    VERSION 1.0
    QML_FILES ${QML_FILE_LIST}
)

install(TARGETS ${MODULE_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/qml)
configure_file(qmldir ${CMAKE_BINARY_DIR}/qml/${MODULE_NAME}/qmldir COPYONLY)
