cmake_minimum_required(VERSION 3.16)

project(GUI LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick QuickControls2)

qt_add_library(${PROJECT_NAME} STATIC)

add_library(MyPedalboard::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON AUTORCC ON)

set_source_files_properties(
  Theme/Colors.qml Theme/Icons.qml Theme/Radiuses.qml Theme/Spaces.qml
  PROPERTIES QT_QML_SINGLETON_TYPE TRUE)

qt_add_qml_module(
  ${PROJECT_NAME}
  NO_PLUGIN
  URI
  MyPedalboardGUIComponents
  VERSION
  1.0
  OUTPUT_DIRECTORY
  ${CMAKE_CURRENT_BINARY_DIR}/MyPedalboardGUIComponents
  QML_FILES
  Theme/Colors.qml
  Theme/Icons.qml
  Theme/Radiuses.qml
  Theme/Spaces.qml
  # SOURCES ButtonCustomControl.cpp ButtonCustomControl.h
)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC Qt6::Core
  PUBLIC Qt6::Qml
  PUBLIC Qt6::Quick
  PUBLIC Qt6::QuickControls2)
