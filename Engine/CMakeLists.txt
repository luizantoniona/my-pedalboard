cmake_minimum_required(VERSION 3.16)

project(Engine LANGUAGES CXX)

set(PROJECT_SOURCES Audio/AudioEngine.cpp Audio/AudioEngine.h Audio/AudioWorker.cpp
                    Audio/AudioWorker.h)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

add_library(MyPedalboard::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

find_package(Qt6 REQUIRED COMPONENTS Core)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC Qt6::Core
  PUBLIC MyPedalboard::Core
  PUBLIC MyPedalboard::Platform)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
