cmake_minimum_required(VERSION 3.20)

if (NOT TARGET nghttp2)
    message(FATAL_ERROR "nghttp2 target not found")
endif()

if (NOT DEFINED PROJECT_SOURCELIB)
    set(PROJECT_SOURCELIB ${PROJECT_NAME}-source)
    set(PROJECT_SOURCELIB ${PROJECT_SOURCELIB} PARENT_SCOPE)
endif()

# =========================================================================== #

add_library(
    ${PROJECT_SOURCELIB} STATIC
    log/report.cpp
    system/handle.cpp
    io/uring/setup.cpp
    io/uring/enter.cpp
    io/uring/sqr.cpp
    io/uring/cqr.cpp
    ip/address.cpp
    ip/v4/address.cpp
    ip/v6/address.cpp
    ip/endpoint.cpp
    ip/v4/endpoint.cpp
    ip/v6/endpoint.cpp
    lib.cpp)

target_compile_options(
    ${PROJECT_SOURCELIB}
    PRIVATE ${CXX_COMPILE_OPTIONS})

target_include_directories(
    ${PROJECT_SOURCELIB}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>)

target_link_libraries(
    ${PROJECT_SOURCELIB}
    PUBLIC nghttp2)

if (DEFINED PROJECT_HEADERS)
    add_dependencies(${PROJECT_SOURCELIB} ${PROJECT_HEADERS})
endif()

if(DEFINED CLANG_TIDY_COMMAND)
    set_property(
        TARGET ${PROJECT_SOURCELIB}
        PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_COMMAND})
endif()
