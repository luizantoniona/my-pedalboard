cmake_minimum_required(VERSION 3.16)

project(Models LANGUAGES CXX)

set(PROJECT_SOURCES EffectChainModel.cpp EffectChainModel.h LevelModel.cpp
                    LevelModel.h TunerModel.cpp TunerModel.h)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

add_library(MyPedalboard::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

find_package(Qt6 REQUIRED COMPONENTS Core Qml)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC Qt6::Core
  PUBLIC Qt6::Qml
  PUBLIC MyPedalboard::Engine)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
