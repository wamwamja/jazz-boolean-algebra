cmake_minimum_required(VERSION 3.22)
project(jazz_boolean_algebra)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")

set(CMAKE_CXX_STANDARD 17)

set(libname jazz_boolean_algebra)
add_subdirectory(src)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/include)

add_subdirectory(tests)

add_subdirectory(playground)

