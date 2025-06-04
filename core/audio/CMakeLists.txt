set(LIBRARY_NAME audio)

file(GLOB SOURCES
    *.cpp
)

file(GLOB HEADERS
    *.h
)

add_library(${LIBRARY_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
