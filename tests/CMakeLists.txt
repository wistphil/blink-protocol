project(blink-tests LANGUAGES CXX)

find_package(GTest CONFIG REQUIRED)

add_executable(blink_tests
    DynamicGroupImplTests.cpp
    LexerTests.cpp
    SchemaBuilderTests.cpp
    StaticGroupImplTests.cpp)

target_link_libraries(blink_tests PRIVATE blink::blink GTest::gtest GTest::gtest_main)

# blinkc tests

set(BLINKC_TESTS_GENERATED_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(BLINKC_TESTS_GENERATED_FILE_NAME ${BLINKC_TESTS_GENERATED_FILE_PATH}/BlinkMessages.hpp)

add_custom_command(
    OUTPUT ${BLINKC_TESTS_GENERATED_FILE_NAME}
    COMMAND cmake -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/etc ${CMAKE_CURRENT_BINARY_DIR}/etc
    COMMAND cmake -E make_directory ${BLINKC_TESTS_GENERATED_FILE_PATH}
    COMMAND blinkc etc/TestSchema.blink ${BLINKC_TESTS_GENERATED_FILE_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS blinkc ${CMAKE_CURRENT_SOURCE_DIR}/etc/TestSchema.blink)

add_executable(blinkc_tests BlinkcTests.cpp ${BLINKC_TESTS_GENERATED_FILE_NAME})

target_include_directories(blinkc_tests
    PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

target_link_libraries(blinkc_tests PRIVATE blink::blink GTest::gtest GTest::gtest_main)

