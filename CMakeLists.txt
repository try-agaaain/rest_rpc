cmake_minimum_required(VERSION 3.1)
project(rest_rpc)

set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")

include_directories(include)

#the thread library of the system.
find_package(Threads REQUIRED)

include(cmake/build.cmake)
include(cmake/develop.cmake)

if (BUILD_UNIT_TESTS)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif ()
if (BUILD_EXAMPLES)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif ()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/mycode)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/basic_code)