cmake_minimum_required(VERSION 3.5.0)
project(pxd-stl VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

################################################################################
## GLOBAL DEFINITIONS

set(CMAKE_BUILD_TYPE Release)

list(APPEND CMAKE_CXX_COMPILER_LAUNCHER "ccache")

set(HEADER_FILES
    sources/array.hpp

    third-party/SIMDString/SIMDString.h
)

set(SOURCE_FILES
    third-party/SIMDString/SIMDString.cpp
    ${HEADER_FILES}
)

include_directories(
    sources
)

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

set(TEST_PROJECT_NAME pxd-stl-test)
set(CMAKE_BUILD_TYPE Debug)

add_executable(${TEST_PROJECT_NAME} main.cpp ${SOURCE_FILES})

target_precompile_headers(
    ${PROJECT_NAME} PRIVATE
    <cstdio>
    <cstdlib>
    ${HEADER_FILES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
