cmake_minimum_required(VERSION 3.14)
project(blink-protocol VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

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
add_subdirectory(examples)

if(NOT CMAKE_SKIP_INSTALL_RULES)
    include(CMakePackageConfigHelpers)

    set(package blink)

    set(blink_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/${package}"
        CACHE PATH "CMake package config location relative to the install prefix")
    mark_as_advanced(blink_INSTALL_CMAKEDIR)

    install(DIRECTORY include/ TYPE INCLUDE)

    install(TARGETS blink blinkc
        EXPORT blink-targets
        RUNTIME COMPONENT blinkc
        ARCHIVE COMPONENT blink
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    configure_package_config_file(
        cmake/blink-config.cmake.in
        blink-config.cmake
        INSTALL_DESTINATION "${blink_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(
        "${package}-config-version.cmake"
        COMPATIBILITY SameMajorVersion
        ARCH_INDEPENDENT)

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/blink-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/blink-config-version.cmake"
        DESTINATION "${blink_INSTALL_CMAKEDIR}")

    install(EXPORT blink-targets
        NAMESPACE blink::
        DESTINATION ${blink_INSTALL_CMAKEDIR})

    if (is_top_level)
        include(CPack)
    endif()
endif()
