cmake_minimum_required(VERSION 3.16)

project(Tests LANGUAGES CXX)

find_package(GTest CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Test)

include(GoogleTest)

# Synthetic input shared by both suites. No Qt, so the Core suite can use it.
set(SUPPORT_SOURCES Support/SignalGenerator.cpp Support/SignalGenerator.h)

add_library(TestSupport STATIC ${SUPPORT_SOURCES})

target_include_directories(
  TestSupport PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)

# Core tests link neither Qt nor RtAudio: if this target ever needs them, the
# portability boundary has been broken.
set(CORE_TEST_SOURCES
    Core/AudioGraphTest.cpp Core/GainTest.cpp Core/PitchDetectorTest.cpp
    Core/RingBufferTest.cpp Core/SmoothedValueTest.cpp)

add_executable(CoreTests ${CORE_TEST_SOURCES})

target_link_libraries(
  CoreTests
  PRIVATE MyPedalboard::Core
  PRIVATE TestSupport
  PRIVATE GTest::gtest
  PRIVATE GTest::gtest_main)

gtest_discover_tests(CoreTests)

# The engine suite brings its own main: the analysis timer that feeds the tuner
# and the meters only fires while a QCoreApplication is processing events.
set(ENGINE_TEST_SOURCES
    Engine/AudioWorkerTest.cpp Engine/TunerIntegrationTest.cpp Engine/main.cpp
    Support/AudioHarness.cpp Support/AudioHarness.h)

add_executable(EngineTests ${ENGINE_TEST_SOURCES})

set_target_properties(EngineTests PROPERTIES AUTOMOC ON)

target_link_libraries(
  EngineTests
  PRIVATE MyPedalboard::Engine
  PRIVATE Qt6::Test
  PRIVATE TestSupport
  PRIVATE GTest::gtest)

# Discovery would have to launch the binary at configure time, before Qt is on
# PATH. One ctest entry running the whole binary avoids that.
add_test(NAME EngineTests COMMAND EngineTests)

set_tests_properties(
  EngineTests
  PROPERTIES ENVIRONMENT_MODIFICATION
             "PATH=path_list_prepend:$<TARGET_FILE_DIR:Qt6::Core>")
