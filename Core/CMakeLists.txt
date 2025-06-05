add_subdirectory(Audio)

set(LIBRARY_NAME Core)

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

target_include_directories(${LIBRARY_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_SOURCE_DIR}/Control
)

target_link_libraries(${LIBRARY_NAME}
    PRIVATE Qt6::Quick
    PUBLIC Control
)
