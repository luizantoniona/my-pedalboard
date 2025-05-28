set(LIBRARY_NAME Effects)

include(${CMAKE_SOURCE_DIR}/cmake/CMakeLibraryConfiguration.cmake)

target_link_libraries(${LIBRARY_NAME} PRIVATE portaudio)
