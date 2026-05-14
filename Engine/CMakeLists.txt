file(GLOB_RECURSE ENGINE_SOURCES
    *.cpp
)

file(GLOB_RECURSE ENGINE_HEADERS
    *.h
)

add_library(EngineLib STATIC
    ${ENGINE_SOURCES}
    ${ENGINE_HEADERS}
)

target_include_directories(EngineLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(EngineLib
    PUBLIC Qt6::Core
    PUBLIC RtAudio::rtaudio
)
