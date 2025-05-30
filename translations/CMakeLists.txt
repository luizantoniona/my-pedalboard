set(TS_FILES
    ${PROJECT_NAME}_en_US.ts
    ${PROJECT_NAME}_pt_BR.ts
)

qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

add_custom_target(translations ALL SOURCES ${TS_FILES})
