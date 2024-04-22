cmake_minimum_required(VERSION 3.5.0)
project(pxd-stl VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

################################################################################
## GLOBAL DEFINITIONS

option(BUILD_TEST_EXECUTABLE "Build test executable" ON)

set(HEADER_FILES
    sources/array.hpp
    sources/checks.hpp
    "sources/string.hpp"

    third-party/SIMDString/SIMDString.h
)

set(LIB_SOURCE_FILES
    third-party/SIMDString/SIMDString.cpp

    ${HEADER_FILES}
)

include_directories(
    sources
)

set(COMMON_STD_HEADERS
    <iostream>
    <cstdlib>
)

# ------------------------------------------------------------------------------------------------------
# -- Project Static Library 

set(CMAKE_BUILD_TYPE "Release")

add_library(${PROJECT_NAME} ${LIB_SOURCE_FILES})

target_precompile_headers(
    ${PROJECT_NAME} PRIVATE
    ${COMMON_STD_HEADERS}
    ${HEADER_FILES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# ------------------------------------------------------------------------------------------------------
# -- Test Executable

IF(BUILD_TEST_EXECUTABLE)
    set(TEST_PROJECT_NAME pxd-stl-test)

    set(CMAKE_BUILD_TYPE "Debug")

    set(TEST_HEADER_FILES
        sources/test/array_tests.hpp
        sources/test/i_test.hpp
        sources/test/test_manager.hpp
        sources/test/test_utils.hpp
    )

    set(TEST_SOURCE_FILES
        ${TEST_HEADER_FILES}
        ${LIB_SOURCE_FILES}
    )

    add_executable(${TEST_PROJECT_NAME} main.cpp ${TEST_SOURCE_FILES})

    target_precompile_headers(
        ${PROJECT_NAME} PRIVATE
        ${COMMON_STD_HEADERS}
        <string>
        <unordered_map>
        <format>
        ${TEST_HEADER_FILES}
        ${HEADER_FILES}
    )

    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
ENDIF(BUILD_TEST_EXECUTABLE)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)