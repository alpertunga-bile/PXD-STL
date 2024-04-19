cmake_minimum_required(VERSION 3.5.0)
project(pxd-stl VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

################################################################################
## GLOBAL DEFINITIONS

set(CMAKE_BUILD_TYPE Debug)

list(APPEND CMAKE_CXX_COMPILER_LAUNCHER "ccache")

set(HEADER_FILES
    includes/array.hpp
)

set(SOURCE_FILES
    main.cpp

    ${HEADER_FILES}
)

include_directories(
    includes
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_precompile_headers(
    ${PROJECT_NAME} PRIVATE
    ${HEADER_FILES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
