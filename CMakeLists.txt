cmake_minimum_required(VERSION 3.16)

project(MyPedalboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(DEFINED ENV{VCPKG_ROOT})
    file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT_CMAKE)
    set(CMAKE_TOOLCHAIN_FILE
        "${VCPKG_ROOT_CMAKE}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING ""
    )
endif()

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Qml)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS QuickControls2)
find_package(RtAudio CONFIG REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

add_subdirectory(Engine)
add_subdirectory(Control)
add_subdirectory(QML)
add_subdirectory(App)

file(GLOB_RECURSE SVG_FILES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    Assets/*.svg
)

qt_add_resources(${PROJECT_NAME}
    PREFIX "/Assets"
    BASE "Assets"
    FILES ${SVG_FILES}
)

