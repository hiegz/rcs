cmake_minimum_required(VERSION 3.20)

if(NOT TARGET gtest)
    message(AUTHOR_WARNING "GTest target not found. Skipping unit test generation ...")
    return()
endif()

include(GoogleTest)
include(CTest)
enable_testing()

if (DEFINED VALGRIND_COMMAND)
    set(DART_CONFIGURATION ${CMAKE_CURRENT_BINARY_DIR}/DartConfiguration.tcl)
    write_file(${DART_CONFIGURATION} "MemoryCheckCommand: ${VALGRIND_COMMAND}")
    write_file(${DART_CONFIGURATION} "MemoryCheckCommandOptions: ${VALGRIND_COMMAND_OPTIONS}" APPEND)
endif()

# =========================================================================== #

set(PROJECT_GTEST ${PROJECT_NAME}-gtest)

add_executable(
    ${PROJECT_GTEST}
    ip/v4/address.cpp
    ip/v6/address.cpp
    ip/v4/endpoint.cpp
    ip/v6/endpoint.cpp
    hex.cpp
    uri.cpp
    test.cpp)

target_compile_options(
    ${PROJECT_GTEST}
    PRIVATE ${CXX_COMPILE_OPTIONS})

target_include_directories(
    ${PROJECT_GTEST}
    PRIVATE $<TARGET_PROPERTY:${PROJECT_SOURCELIB},INCLUDE_DIRECTORIES>)

target_link_libraries(
    ${PROJECT_GTEST}
    PRIVATE ${PROJECT_SOURCELIB}
    PRIVATE GTest::gtest)

if (DEFINED PROJECT_HEADERS)
    add_dependencies(${PROJECT_GTEST} ${PROJECT_HEADERS})
endif()

gtest_discover_tests(${PROJECT_GTEST})

if(DEFINED CLANG_TIDY_COMMAND)
    set_property(
        TARGET ${PROJECT_GTEST}
        PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_COMMAND})
endif()
