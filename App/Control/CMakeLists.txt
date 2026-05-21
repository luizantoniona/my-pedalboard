file(GLOB_RECURSE CONTROL_SOURCES
    *.cpp
)

file(GLOB_RECURSE CONTROL_HEADERS
    *.h
)

add_library(ControlLib STATIC
    ${CONTROL_SOURCES}
    ${CONTROL_HEADERS}
)

target_include_directories(ControlLib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(ControlLib
    PUBLIC EngineLib
    PUBLIC Qt6::Qml
    PUBLIC Qt6::Quick
)
