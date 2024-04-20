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

set(SOURCE_FILES
    third-party/SIMDString/SIMDString.cpp
    ${HEADER_FILES}
)

include_directories(
    sources
)

# ------------------------------------------------------------------------------------------------------
# -- Project Static Library 

set(CMAKE_BUILD_TYPE "Release")

add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_precompile_headers(
    ${PROJECT_NAME} PRIVATE
    <cstdio>
    <cstdlib>
    ${HEADER_FILES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# ------------------------------------------------------------------------------------------------------
# -- Test Executable

IF(BUILD_TEST_EXECUTABLE)
    set(TEST_PROJECT_NAME pxd-stl-test)

    set(CMAKE_BUILD_TYPE "Debug")

    set(TEST_FILES
        sources/test/array_tests.hpp
        sources/test/i_test.hpp
        sources/test/test_manager.hpp
    )

    add_executable(${TEST_PROJECT_NAME} main.cpp ${SOURCE_FILES} ${TEST_FILES})

    target_precompile_headers(
        ${PROJECT_NAME} PRIVATE
        <cstdio>
        <cstdlib>
        <string>
        <unordered_map>
        ${HEADER_FILES}
        ${TEST_FILES}
    )

    target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
ENDIF(BUILD_TEST_EXECUTABLE)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)