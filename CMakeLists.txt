cmake_minimum_required(VERSION 3.5.0)
project(pxd-stl VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

# ------------------------------------------------------------------------------------
# -- GLOBAL DEFINITIONS

IF(NOT MSVC)
    set(CMAKE_BUILD_TYPE "Debug")
ENDIF(NOT MSVC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
endif(NOT MSVC)

if(NOT MSVC)
    add_compile_definitions(CMAKE_CXX_INCLUDE_WHAT_YOU_USE="include-what-you-use")
endif(NOT MSVC)

option(PXD_STL_BUILD_TEST_EXECUTABLE "Build test executable" ON)

set(PXD_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(PXD_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party)
set(PXD_TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

# check if used with subdirectory or main project
IF(NOT ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}"))
    set(PXD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes PARENT_SCOPE)
ELSE()
    set(PXD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)
ENDIF()

set(HEADER_FILES
    ${PXD_INCLUDE_DIR}/ds/array.hpp
    ${PXD_INCLUDE_DIR}/ds/linked_list.hpp
    ${PXD_INCLUDE_DIR}/ds/binary_search_tree.hpp
    ${PXD_INCLUDE_DIR}/ds/stack.hpp
    ${PXD_INCLUDE_DIR}/ds/queue.hpp
    ${PXD_INCLUDE_DIR}/handle.hpp
    ${PXD_INCLUDE_DIR}/ds/dynamic_array.hpp
    ${PXD_INCLUDE_DIR}/matrix.hpp
    ${PXD_INCLUDE_DIR}/ds/double_linked_list.hpp
    ${PXD_INCLUDE_DIR}/ds/xor_double_linked_list.hpp
    ${PXD_INCLUDE_DIR}/ds/priority_queue.hpp
    ${PXD_INCLUDE_DIR}/ds/dheap.hpp
    
    ${PXD_INCLUDE_DIR}/regex.hpp
    ${PXD_INCLUDE_DIR}/logger.hpp
    ${PXD_INCLUDE_DIR}/checks.hpp
    "${PXD_INCLUDE_DIR}/string.hpp"

    ${PXD_THIRD_PARTY_DIR}/SIMDString/SIMDString.h
    ${PXD_THIRD_PARTY_DIR}/re2/re2/re2.h
    ${PXD_THIRD_PARTY_DIR}/fmt/include/fmt/core.h
    ${PXD_THIRD_PARTY_DIR}/fmt/include/fmt/os.h
)

set(LIB_SOURCE_FILES
    ${PXD_THIRD_PARTY_DIR}/SIMDString/SIMDString.cpp

    ${PXD_SOURCE_DIR}/ds/array.cpp
    ${PXD_SOURCE_DIR}/ds/linked_list.cpp
    ${PXD_SOURCE_DIR}/ds/binary_search_tree.cpp
    ${PXD_SOURCE_DIR}/logger.cpp
    ${PXD_SOURCE_DIR}/handle.cpp
    ${PXD_SOURCE_DIR}/ds/dynamic_array.cpp
    ${PXD_SOURCE_DIR}/matrix.cpp
    ${PXD_SOURCE_DIR}/regex.cpp
    ${PXD_SOURCE_DIR}/ds/double_linked_list.cpp
    ${PXD_SOURCE_DIR}/ds/xor_double_linked_list.cpp
    ${PXD_SOURCE_DIR}/ds/priority_queue.cpp
    ${PXD_SOURCE_DIR}/utility.cpp
    ${PXD_SOURCE_DIR}/ds/dheap.cpp

    ${HEADER_FILES}
)

include_directories(
   ${PXD_INCLUDE_DIR} 
   ${PXD_INCLUDE_DIR}/ds
   ${PXD_THIRD_PARTY_DIR}/fmt/include/fmt
   ${PXD_THIRD_PARTY_DIR}/SIMDString
   ${PXD_THIRD_PARTY_DIR}/re2/re2
)

set(COMMON_STD_HEADERS
    <stdlib.h>
    <cstdint>
    <vector>
    <algorithm>

    # Logger is using these headers
    <filesystem>
    <fstream>
)

set(ABSL_PROPAGATE_CXX_STD ON)
set(ABSL_ENABLE_INSTALL ON)

add_subdirectory(${PXD_THIRD_PARTY_DIR}/abseil-cpp)
add_subdirectory(${PXD_THIRD_PARTY_DIR}/re2)
add_subdirectory(${PXD_THIRD_PARTY_DIR}/fmt)

set(LIBS_TO_LINK
    re2::re2
    fmt::fmt
)

# ------------------------------------------------------------------------------------------------------
# -- Project Static Library 

add_library(${PROJECT_NAME} ${LIB_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${LIBS_TO_LINK})

target_precompile_headers(
    ${PROJECT_NAME} PRIVATE
    ${COMMON_STD_HEADERS}
    ${HEADER_FILES}
)

# ------------------------------------------------------------------------------------------------------
# -- Test Executable

IF(PXD_STL_BUILD_TEST_EXECUTABLE)
    set(TEST_PROJECT_NAME pxd-stl-test)

    set(TEST_HEADER_FILES
        ${PXD_TEST_DIR}/priority_queue_tests.hpp
        ${PXD_TEST_DIR}/xor_double_linked_list_tests.hpp
        ${PXD_TEST_DIR}/double_linked_list_tests.hpp
        ${PXD_TEST_DIR}/matrix_tests.hpp
        ${PXD_TEST_DIR}/dynamic_array_tests.hpp
        ${PXD_TEST_DIR}/queue_tests.hpp
        ${PXD_TEST_DIR}/stack_tests.hpp
        ${PXD_TEST_DIR}/binary_search_tree_tests.hpp
        ${PXD_TEST_DIR}/linked_list_tests.hpp
        ${PXD_TEST_DIR}/array_tests.hpp
        ${PXD_TEST_DIR}/i_test.hpp
        ${PXD_TEST_DIR}/test_manager.hpp
        ${PXD_TEST_DIR}/test_utils.hpp
    )

    set(TEST_SOURCE_FILES
        ${TEST_HEADER_FILES}
        ${LIB_SOURCE_FILES}
    )

    add_executable(${TEST_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/main.cpp ${TEST_SOURCE_FILES})

    target_link_libraries(${TEST_PROJECT_NAME} ${LIBS_TO_LINK})

    target_precompile_headers(
        ${PROJECT_NAME} PRIVATE
        ${COMMON_STD_HEADERS}
        <unordered_map>
        <utility>
        ${TEST_HEADER_FILES}
        ${HEADER_FILES}
    )
ENDIF()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)