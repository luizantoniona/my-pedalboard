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
    DSP/Effect/Delay/Delay.cpp
    DSP/Effect/Delay/Delay.h
    DSP/Effect/Distortion/HighGain.cpp
    DSP/Effect/Distortion/HighGain.h
    DSP/Effect/Distortion/Overdrive.cpp
    DSP/Effect/Distortion/Overdrive.h
    DSP/Effect/Dynamics/NoiseGate.cpp
    DSP/Effect/Dynamics/NoiseGate.h
    DSP/Effect/Filter/Biquad.cpp
    DSP/Effect/Filter/Biquad.h
    DSP/Effect/Filter/Equalizer.cpp
    DSP/Effect/Filter/Equalizer.h
    DSP/Effect/Modulation/Chorus.cpp
    DSP/Effect/Modulation/Chorus.h
    DSP/Effect/Reverb/Reverb.cpp
    DSP/Effect/Reverb/Reverb.h
    DSP/Effect/Utility/Gain.cpp
    DSP/Effect/Utility/Gain.h
    DSP/EffectNode.cpp
    DSP/EffectNode.h
    DSP/ParameterInfo.h
    Graph/AudioGraph.cpp
    Graph/AudioGraph.h
    Project/NodeRegistry.cpp
    Project/NodeRegistry.h
    Project/ParameterOverride.h
    Project/PedalDefinition.h
    Project/PedalRegistry.cpp
    Project/PedalRegistry.h
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
