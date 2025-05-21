cmake_minimum_required(VERSION 3.14)

project(my-pedalboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick LinguistTools)

list(APPEND QML_IMPORT_PATH ${CMAKE_SOURCE_DIR}/qml)
set(QML_IMPORT_PATH ${QML_IMPORT_PATH} CACHE STRING "Custom QML import paths" FORCE)

add_subdirectory(third_party/portaudio EXCLUDE_FROM_ALL)
add_subdirectory(translations)
add_subdirectory(qml)

set(SOURCES
    main.cpp
    ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    include(cmake/CMakeQt6.cmake)
else()
    include(cmake/CMakeQt5.cmake)
endif()
