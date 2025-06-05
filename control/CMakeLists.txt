set(LIBRARY_NAME control)

file(GLOB_RECURSE SOURCES
    *.cpp
)

file(GLOB_RECURSE HEADERS
    *.h
)

add_library(${LIBRARY_NAME} STATIC
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
