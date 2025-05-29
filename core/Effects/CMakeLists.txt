set(LIBRARY_NAME Effects)

file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

file(GLOB HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_library(${LIBRARY_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME}
    PRIVATE Qt6::Core
    PRIVATE portaudio
)
