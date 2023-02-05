cmake_minimum_required(VERSION 3.14)
project(blink-protocol LANGUAGES CXX)

find_package(byte_order CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

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
        include/blink/CharReader.hpp
        include/blink/DataArea.hpp
        src/DataArea.cpp
        include/blink/DynamicGroupImpl.hpp
        src/DynamicGroupImpl.cpp
        include/blink/DynamicGroupPreamble.hpp
        src/DynamicGroupPreamble.cpp
        include/blink/GroupImpl.hpp
        src/GroupImpl.cpp
        include/blink/Lexer.hpp
        src/Lexer.cpp
        include/blink/PrintHelpers.hpp
        src/PrintHelpers.cpp
        include/blink/Schema.hpp
        src/Schema.cpp
        include/blink/SchemaBuilder.hpp
        src/SchemaBuilder.cpp
        include/blink/StaticGroupImpl.hpp
        src/StaticGroupImpl.cpp
        include/blink/Sequence.hpp
        include/blink/SequenceImpl.hpp
        src/SequenceImpl.cpp
        include/blink/Token.hpp
        src/Token.cpp
        include/blink/Util.hpp)

target_link_libraries(blink PRIVATE byte_order::byte_order fmt::fmt)

add_subdirectory(tools)
add_subdirectory(tests)
