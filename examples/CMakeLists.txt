cmake_minimum_required(VERSION 3.14)
project(blink-protocol-examples LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(byte_order CONFIG REQUIRED)
    find_package(fmt CONFIG REQUIRED)
    find_package(blink CONFIG REQUIRED)
endif()

set(BLINK_EXAMPLES_GENERATED_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(BLINK_EXAMPLES_GENERATED_FILE_NAMES
    ${BLINK_EXAMPLES_GENERATED_FILE_PATH}/ExampleSchema.hpp
    ${BLINK_EXAMPLES_GENERATED_FILE_PATH}/ExampleSchema.cpp)

add_custom_command(
    OUTPUT ${BLINK_EXAMPLES_GENERATED_FILE_NAMES}
    COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/etc ${CMAKE_CURRENT_BINARY_DIR}/etc
    COMMAND cmake -E make_directory ${BLINK_EXAMPLES_GENERATED_FILE_PATH}
    COMMAND blink::blinkc etc/ExampleSchema.blink ${BLINK_EXAMPLES_GENERATED_FILE_PATH}/
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS blink::blinkc ${CMAKE_CURRENT_SOURCE_DIR}/etc/ExampleSchema.blink)

add_custom_target(generated_schema_files DEPENDS ${BLINK_EXAMPLES_GENERATED_FILE_NAMES})

add_subdirectory(finance)
add_subdirectory(transactions)
