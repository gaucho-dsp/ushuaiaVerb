cmake_minimum_required(VERSION 3.22)
project(UshuaiaVerb)

set(CMAKE_CXX_STANDARD 17)


set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
include(cmake/cpm.cmake)
add_executable(UshuaiaVerb main.cpp)
