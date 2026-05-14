find_package(GTest REQUIRED)

add_executable(MyPedalboardTests
    test_example.cpp
)

target_link_libraries(MyPedalboardTests
    PRIVATE EngineLib
    PRIVATE GTest::gtest_main
)

add_test(NAME MyPedalboardTests COMMAND MyPedalboardTests)
