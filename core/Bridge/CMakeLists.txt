set(LIBRARY_NAME Bridge)

file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(${LIBRARY_NAME} STATIC
    ${SOURCES}
)

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${LIBRARY_NAME} PRIVATE portaudio)
