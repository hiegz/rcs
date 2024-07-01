cmake_minimum_required(VERSION 3.20)

if (NOT DEFINED PROJECT_SOURCELIB)
    set(PROJECT_SOURCELIB ${PROJECT_NAME}-source)
    set(PROJECT_SOURCELIB ${PROJECT_SOURCELIB} PARENT_SCOPE)
endif()

# =========================================================================== #

add_library(
    ${PROJECT_SOURCELIB} STATIC
    log/report.cpp
    system/handle.cpp
    lib.cpp)

target_compile_options(
    ${PROJECT_SOURCELIB}
    PRIVATE ${CXX_COMPILE_OPTIONS})

target_include_directories(
    ${PROJECT_SOURCELIB}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)

if (DEFINED PROJECT_HEADERS)
    add_dependencies(${PROJECT_SOURCELIB} ${PROJECT_HEADERS})
endif()

if(DEFINED CLANG_TIDY_COMMAND)
    set_property(
        TARGET ${PROJECT_SOURCELIB}
        PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_COMMAND})
endif()
