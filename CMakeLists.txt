cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
  file(TO_CMAKE_PATH "$ENV{VCPKG_ROOT}" VCPKG_ROOT_CMAKE)
  set(CMAKE_TOOLCHAIN_FILE
      "${VCPKG_ROOT_CMAKE}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project(MyPedalboard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(GUI)
list(APPEND QML_IMPORT_PATH "${CMAKE_BINARY_DIR}/GUI/UI")

add_subdirectory(Engine)

add_subdirectory(Models)

add_subdirectory(App)

set(QML_IMPORT_PATH
    "${QML_IMPORT_PATH}"
    CACHE STRING "QML import paths" FORCE)
