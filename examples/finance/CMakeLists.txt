set_source_files_properties(
    ${BLINK_EXAMPLES_GENERATED_FILE_NAMES}
    PROPERTIES GENERATED TRUE
)

add_executable(finance_example main.cpp ${BLINK_EXAMPLES_GENERATED_FILE_NAMES})
add_dependencies(finance_example generated_schema_files)

target_include_directories(finance_example
    PRIVATE
        "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>")

target_link_libraries(finance_example
    PRIVATE
        blink::blink)

