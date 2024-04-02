cmake_minimum_required(VERSION 3.20)

# Save the timestamp of the current build
string(TIMESTAMP CMAKE_BUILD_TIMESTAMP "%Y-%m-%d %H:%M")

find_program(GPP_EXE g++)
if (GPP_EXE)
    message(STATUS "Found g++ : ${GPP_EXE}.")
elseif (NOT GPP_EXE)
    message(FATAL_ERROR "g++ not found in $PATH.")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if (CLANG_TIDY_EXE)
        message(STATUS "Found clang-tidy : ${CLANG_TIDY_EXE}.")
        set(CLANG_TIDY_COMMAND ${CLANG_TIDY_EXE}
              --config-file ${CMAKE_SOURCE_DIR}/.clang-tidy
              -p ${CMAKE_BINARY_DIR}
              --quiet)
    elseif(NOT CLANG_TIDY_EXE)
        message(AUTHOR_WARNING "clang-tidy not found in $PATH.")
    endif()
endif()

# === Project =============================================================== #

project(
    rcs
    VERSION 0.1.0
    LANGUAGES CXX)

# === External dependencies ================================================= #

add_subdirectory(extern)

# === C++ settings ========================================================== #

set(CMAKE_CXX_COMPILER ${GPP_EXE})
set(CMAKE_CXX_STANDARD 20) # C++20

set(CXX_COMPILE_OPTIONS
    -Werror
    -Wall
    -Wextra
    -Wpedantic)

# Export compile commands for LSPs
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
endif()

# === Subdirectories ======================================================== #

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)

# === Executable ============================================================ #

set(PROJECT_EXECUTABLE ${PROJECT_NAME})

add_executable(
    ${PROJECT_EXECUTABLE}
    main.cpp)

target_compile_options(
    ${PROJECT_EXECUTABLE}
    PRIVATE ${CXX_COMPILE_OPTIONS})

target_compile_definitions(
    ${PROJECT_EXECUTABLE}
    PRIVATE -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    PRIVATE -DCMAKE_PROJECT_NAME="${CMAKE_PROJECT_NAME}"
    PRIVATE -DCMAKE_PROJECT_VERSION="${CMAKE_PROJECT_VERSION}"
    PRIVATE -DCMAKE_BUILD_TIMESTAMP="${CMAKE_BUILD_TIMESTAMP}")

target_include_directories(
    ${PROJECT_EXECUTABLE}
    PRIVATE $<TARGET_PROPERTY:${PROJECT_SOURCELIB},INCLUDE_DIRECTORIES>)

target_link_libraries(
    ${PROJECT_EXECUTABLE}
    PRIVATE ${PROJECT_SOURCELIB})

if (DEFINED PROJECT_HEADERS)
    add_dependencies(${PROJECT_EXECUTABLE} ${PROJECT_HEADERS})
endif()

if(DEFINED CLANG_TIDY_COMMAND)
    set_property(
        TARGET ${PROJECT_EXECUTABLE}
        PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_COMMAND})
endif()
