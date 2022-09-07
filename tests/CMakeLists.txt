project(blink-tests LANGUAGES CXX)

find_package(GTest CONFIG REQUIRED)

add_executable(blink_tests
    StaticGroupImplTests.cpp)

target_link_libraries(blink_tests PRIVATE blink::blink GTest::gtest GTest::gtest_main)

