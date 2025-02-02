cmake_minimum_required(VERSION 3.22)
project(AudioPluginTest)
enable_testing()
add_executable(${PROJECT_NAME}
        source/AudioProcessorTest.cpp)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../plugin/include
        ${JUCE_SOURCE_DIR}/modules
        ${GOOGLETEST_SOURCE_DIR}/googletest/include)
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        AudioPlugin
        GTest::gtest_main)
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(GoogleTest)

if (CMAKE_GENERATOR STREQUAL Xcode)
    gtest_discover_tests(${PROJECT_NAME} DISCOVERY_MODE PRE_TEST)
else()
    gtest_discover_tests(${PROJECT_NAME})
endif()