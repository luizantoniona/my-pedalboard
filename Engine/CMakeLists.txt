cmake_minimum_required(VERSION 3.16)

project(Engine LANGUAGES CXX)

set(PROJECT_SOURCES
    RegisterEngineTypes.cpp
    RegisterEngineTypes.h
    Audio/AudioDevice.cpp
    Audio/AudioDevice.h
    Audio/AudioEngine.cpp
    Audio/AudioEngine.h
    Audio/AudioGraph.cpp
    Audio/AudioGraph.h
    Audio/AudioWorker.cpp
    Audio/AudioWorker.h
    DSP/AudioNode.h
    DSP/DSPModule.cpp
    DSP/DSPModule.h
    DSP/EffectNode.cpp
    DSP/EffectNode.h
    DSP/Effects/Gain.cpp
    DSP/Effects/Gain.h
    Project/NodeFactory.cpp
    Project/NodeFactory.h
    Project/PresetManager.cpp
    Project/PresetManager.h
    Project/Serializer.cpp
    Project/Serializer.h
    Transport/Transport.cpp
    Transport/Transport.h
    Utils/AtomicValue.h
    Utils/AudioMath.h
    Utils/RingBuffer.h)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(RtAudio CONFIG REQUIRED)

target_link_libraries(
  ${PROJECT_NAME}
  PUBLIC Qt6::Core
  PUBLIC RtAudio::rtaudio)

add_library(MyPedalboard::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
