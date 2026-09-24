cmake_minimum_required(VERSION 3.16)

project(Core LANGUAGES CXX)

set(PROJECT_SOURCES
    Analysis/LevelMeter.cpp
    Analysis/LevelMeter.h
    Analysis/NoteHelper.cpp
    Analysis/NoteHelper.h
    Analysis/PitchDetector.cpp
    Analysis/PitchDetector.h
    DSP/AudioBuffer.h
    DSP/AudioNode.h
    DSP/DSPModule.h
    DSP/Effect/Utility/Gain.cpp
    DSP/Effect/Utility/Gain.h
    DSP/EffectNode.cpp
    DSP/EffectNode.h
    Graph/AudioGraph.cpp
    Graph/AudioGraph.h
    Project/NodeRegistry.cpp
    Project/NodeRegistry.h
    Utils/AtomicValue.h
    Utils/AudioMath.h
    Utils/DenormalGuard.h
    Utils/RingBuffer.h
    Utils/SmoothedValue.h)

add_library(${PROJECT_NAME} STATIC ${PROJECT_SOURCES})

add_library(MyPedalboard::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Core is the portability boundary: it runs on desktop, embedded Linux and a
# Cortex-M microcontroller. Linking Qt or RtAudio here breaks the MCU target.
target_link_libraries(${PROJECT_NAME})

target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
