add_subdirectory(audio)

set(LIBRARY_NAME core)

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
    PUBLIC ${CMAKE_SOURCE_DIR}/control
)

target_link_libraries(${LIBRARY_NAME}
    PRIVATE Qt6::Quick
    PUBLIC control
)
