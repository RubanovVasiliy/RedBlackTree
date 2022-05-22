cmake_minimum_required(VERSION 3.20)
project(rgr_csc)

set(CMAKE_CXX_STANDARD 20)

include(cmake/CompileOptions.cmake)

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(test)