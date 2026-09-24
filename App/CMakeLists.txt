cmake_minimum_required(VERSION 3.16)

project(MyPedalboard LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

set(PROJECT_SOURCES
    AppContext.cpp
    AppContext.h
    RegisterAppTypes.cpp
    RegisterAppTypes.h
    Application/Home/HomePageControl.cpp
    Application/Home/HomePageControl.h
    Application/Settings/SettingsPageControl.cpp
    Application/Settings/SettingsPageControl.h)

qt_add_executable(${PROJECT_NAME} main.cpp ${PROJECT_SOURCES})

qt_add_qml_module(
  ${PROJECT_NAME}
  URI
  MyPedalboardAppComponents
  VERSION
  1.0
  QML_FILES
  Main.qml
  Application/ApplicationWindow.qml
  Application/About/AboutPage.qml
  Application/Home/HomePage.qml
  Application/Settings/SettingsPage.qml)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt6::QuickControls2
  PRIVATE MyPedalboard::Engine
  PRIVATE MyPedalboard::Models
  PRIVATE MyPedalboard::GUI)

target_include_directories(
  ${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
