cmake_minimum_required(VERSION 3.16)

project(Models LANGUAGES CXX)

set(PROJECT_SOURCES AudioDeviceModel.cpp AudioDeviceModel.h
                    EffectChainModel.cpp EffectChainModel.h)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

find_package(Qt6 REQUIRED COMPONENTS Core Qml)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC Qt6::Core
  PUBLIC Qt6::Qml)

add_library(MyPedalboard::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
