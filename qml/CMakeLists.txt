set(MODULE_NAME qml)

# file(GLOB QML_FILES_LIST
#     RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
#     CONFIGURE_DEPENDS
#     "*.qml"
# )

qt_add_qml_module(${MODULE_NAME}
    URI ${MODULE_NAME}
    VERSION 1.0
    QML_FILES
        ${QML_FILES_LIST}
)

add_subdirectory(Atoms)
add_subdirectory(Molecules)
add_subdirectory(Organisms)
add_subdirectory(Pages)

set(QML_MODULES ${QML_MODULES} ${MODULE_NAME} PARENT_SCOPE)
