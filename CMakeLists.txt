cmake_minimum_required(VERSION 3.14)

project(my-pedalboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick Gui LinguistTools)
find_package(PortAudio REQUIRED)


set(TS_FILES
    translations/my-pedalboard_en_US.ts
)

add_subdirectory(qml)
add_subdirectory(qml/screen)
add_subdirectory(qml/component)

set(SOURCES
    main.cpp
    qml.qrc
    ${TS_FILES}
)

qt_add_executable(my-pedalboard
    MANUAL_FINALIZATION
    ${SOURCES}
)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(my-pedalboard
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Gui
    PRIVATE PortAudio::PortAudio
)

set_target_properties(my-pedalboard PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS my-pedalboard
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_import_qml_plugins(my-pedalboard)
qt_finalize_executable(my-pedalboard)
