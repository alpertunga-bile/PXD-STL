cmake_minimum_required(VERSION 3.5.0)
project(pxd-stl VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(!MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
endif(!MSVC)

# ------------------------------------------------------------------------------------
# -- GLOBAL DEFINITIONS

option(PXD_STL_BUILD_TEST_EXECUTABLE "Build test executable" ON)

set(HEADER_FILES
    sources/array.hpp
    sources/linked_list.hpp
    sources/binary_search_tree.hpp
    
    sources/logger.hpp
    sources/checks.hpp
    "sources/string.hpp"

    third-party/SIMDString/SIMDString.h
)

set(LIB_SOURCE_FILES
    third-party/SIMDString/SIMDString.cpp

    sources/array.cpp
    sources/linked_list.cpp
    sources/logger.cpp

    ${HEADER_FILES}
)

include_directories(
    sources
)

set(COMMON_STD_HEADERS
    <iostream>
    <cstdlib>

    # Logger is using these headers
    <filesystem>
    <format>
    <fstream>

    # SIMDString is using these headers
    <algorithm>
    <assert.h>
    <cstddef>
    <cstring>
    <errno.h>
    <initializer_list>
    <ios>
    <iterator>
    <limits>
    <stdint.h>
    <string>
    <string_view>
    <immintrin.h>
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

# ------------------------------------------------------------------------------------------------------
# -- Test Executable

IF(PXD_STL_BUILD_TEST_EXECUTABLE)
    set(TEST_PROJECT_NAME pxd-stl-test)

    set(CMAKE_BUILD_TYPE "Debug")

    set(TEST_HEADER_FILES
        sources/test/binary_search_tree_tests.hpp
        sources/test/linked_list_tests.hpp
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
        <unordered_map>
        <utility>
        ${TEST_HEADER_FILES}
        ${HEADER_FILES}
    )
ENDIF(PXD_STL_BUILD_TEST_EXECUTABLE)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)