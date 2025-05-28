set(LIBRARY_NAME Audio)

include(${CMAKE_SOURCE_DIR}/cmake/CMakeLibraryConfiguration.cmake)

target_link_libraries(${LIBRARY_NAME} PRIVATE portaudio)
