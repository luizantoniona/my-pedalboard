cmake_minimum_required(VERSION 3.16)

if(DEFINED ENV{VCPKG_ROOT})
    file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT_CMAKE)
    set(CMAKE_TOOLCHAIN_FILE
        "${VCPKG_ROOT_CMAKE}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING ""
    )
endif()

project(my-pedalboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(RtAudio CONFIG REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

add_subdirectory(Translations)
add_subdirectory(Control)
add_subdirectory(Engine)
add_subdirectory(QML)

qt_add_executable(${PROJECT_NAME}
    main.cpp
)

qt_add_qml_module(${PROJECT_NAME}
    URI main
    VERSION 1.0
    QML_FILES Main.qml
)

file(GLOB_RECURSE SVG_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Assets/*.svg
)

qt_add_resources(${PROJECT_NAME}
    PREFIX "/Assets"
    BASE "Assets"
    FILES ${SVG_FILES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PRIVATE ${QML_MODULES}
    PRIVATE Control
    PRIVATE Engine
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

message(STATUS "Toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
