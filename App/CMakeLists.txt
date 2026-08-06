cmake_minimum_required(VERSION 3.16)

project(MyPedalboard LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

set(PROJECT_SOURCES
    Control/AudioControl.cpp Control/AudioControl.h
    Control/RegisterControlTypes.cpp Control/RegisterControlTypes.h)

qt_add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCES})

qt_add_qml_module(
  ${PROJECT_NAME}
  URI
  MyPedalboardAppComponents
  VERSION
  1.0
  QML_FILES
  Main.qml
  ApplicationWindow.qml
  Pages/AboutPage.qml
  Pages/HomePage.qml
  Pages/SettingsPage.qml)

file(
  GLOB_RECURSE SVG_FILES
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  Assets/*.svg)

qt_add_resources(
  ${PROJECT_NAME}
  PREFIX
  "/Assets"
  BASE
  "Assets"
  FILES
  ${SVG_FILES})

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt6::QuickControls2
  PRIVATE MyPedalboard::Engine
  PRIVATE MyPedalboard::Models
  PRIVATE MyPedalboard::GUI)
