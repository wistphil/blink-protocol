cmake_minimum_required(VERSION 3.14)
project(blink-protocol LANGUAGES CXX)

find_package(byte_order CONFIG REQUIRED)

add_library(blink)
add_library(blink::blink ALIAS blink)

target_include_directories(blink
        PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
            "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/src")

target_sources(blink
    PRIVATE
        include/blink/DynamicGroupImpl.hpp
        include/blink/DynamicGroupPreamble.hpp
        include/blink/StaticGroupImpl.hpp
        src/StaticGroupImpl.cpp)

target_link_libraries(blink PRIVATE byte_order::byte_order)

add_subdirectory(tools)
add_subdirectory(tests)