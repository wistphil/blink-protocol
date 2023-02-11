cmake_minimum_required(VERSION 3.14)
project(blink-protocol-example LANGUAGES CXX)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${PROJECT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(byte_order CONFIG REQUIRED)
    find_package(fmt CONFIG REQUIRED)
    find_package(blink CONFIG REQUIRED)
endif()

set(BLINKC_EXAMPLE_GENERATED_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(BLINKC_EXAMPLE_GENERATED_FILE_NAMES
    ${BLINKC_EXAMPLE_GENERATED_FILE_PATH}/ExampleSchema.hpp
    ${BLINKC_EXAMPLE_GENERATED_FILE_PATH}/ExampleSchema.cpp)

add_custom_command(
    OUTPUT ${BLINKC_EXAMPLE_GENERATED_FILE_NAMES}
    COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/etc ${CMAKE_CURRENT_BINARY_DIR}/etc
    COMMAND cmake -E make_directory ${BLINKC_EXAMPLE_GENERATED_FILE_PATH}
    COMMAND blink::blinkc etc/ExampleSchema.blink ${BLINKC_EXAMPLE_GENERATED_FILE_PATH}/
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS blink::blinkc ${CMAKE_CURRENT_SOURCE_DIR}/etc/ExampleSchema.blink)

add_executable(example example.cpp ${BLINKC_EXAMPLE_GENERATED_FILE_NAMES})

target_include_directories(example
    PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

target_link_libraries(example
    PRIVATE
        blink::blink)

