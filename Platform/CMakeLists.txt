cmake_minimum_required(VERSION 3.16)

project(Platform LANGUAGES CXX)

set(PROJECT_SOURCES
    AudioBackend.h
    AudioDeviceInfo.h
    Null/NullAudioBackend.cpp
    Null/NullAudioBackend.h
    RtAudio/RtAudioBackend.cpp
    RtAudio/RtAudioBackend.h)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

add_library(MyPedalboard::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

find_package(RtAudio CONFIG REQUIRED)

# RtAudio is PRIVATE on purpose: only the backend implementation sees it.
target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC MyPedalboard::Core
  PRIVATE RtAudio::rtaudio)

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
