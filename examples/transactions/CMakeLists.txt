set_source_files_properties(
    ${BLINK_EXAMPLES_GENERATED_FILE_NAMES}
    PROPERTIES GENERATED TRUE
)

add_executable(transactions_example main.cpp ${BLINK_EXAMPLES_GENERATED_FILE_NAMES})
add_dependencies(transactions_example generated_schema_files)

target_include_directories(transactions_example
    PRIVATE
        "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>")

target_link_libraries(transactions_example
    PRIVATE
        blink::blink)

