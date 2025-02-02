cmake_minimum_required(VERSION 3.22)
project(UshuaiaVerb)
set(CMAKE_CXX_STANDARD 23)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
include(cmake/cpm.cmake)
CPMAddPackage(
        NAME JUCE
        GITHUB_REPOSITORY juce-framework/JUCE
        GIT_TAG 8.0.6
        VERSION 8.0.6
        SOURCE_DIR ${LIB_DIR}/juce
)
CPMAddPackage(
        NAME GOOGLETEST
        GITHUB_REPOSITORY google/googletest
        VERSION 1.15.2
        SOURCE_DIR ${LIB_DIR}/googletest
        OPTIONS
            "INSTALL_GTEST OFF"
            "gtest_force_shared_crt ON"
)

if (MSVC)
    add_compile_options(/Wall /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

enable_testing()

add_subdirectory(plugin)
add_subdirectory(test)

