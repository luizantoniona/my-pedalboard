set(LIBRARY_NAME Engine)

file(GLOB SOURCES
    *.cpp
    Audio/*.cpp
)

file(GLOB HEADERS
    *.h
    Audio/*.h
)

add_library(${LIBRARY_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${LIBRARY_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${LIBRARY_NAME}
    PRIVATE Qt6::Core
    PRIVATE Qt6::Quick
    PRIVATE RtAudio::rtaudio
)
