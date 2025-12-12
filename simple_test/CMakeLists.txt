cmake_minimum_required(VERSION 3.22)
project(SimpleGUITest VERSION 0.1.0)

# Find CPM package manager
find_program(CPM_SCRIPT "cpm.cmake" PATHS "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" NO_DEFAULT_PATH)
if(CPM_SCRIPT)
    include(${CPM_SCRIPT})
else()
    message(FATAL_ERROR "CPM not found")
endif()

# Add JUCE
CPMAddPackage(
    NAME JUCE
    GIT_TAG 8.0.6
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../libs/juce
    OPTIONS "JUCE_ENABLE_MODULE_SOURCE_GROUPS ON"
)

juce_add_plugin(${PROJECT_NAME}
        COMPANY_NAME gaucho_dsp
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        PLUGIN_MANUFACTURER_CODE GDSP
        PLUGIN_CODE TEST
        FORMATS VST3
        PRODUCT_NAME "SimpleGUITest"
)

target_sources(${PROJECT_NAME}
        PRIVATE
        source/PluginEditor.cpp
        source/PluginProcessor.cpp
)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        include
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
            juce::juce_audio_utils
        PUBLIC
            juce::juce_recommended_config_flags
            juce::juce_recommended_lto_flags
            juce::juce_recommended_warning_flags
)

target_compile_definitions(${PROJECT_NAME}
        PUBLIC
            JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1
            JUCE_WEB_BROWSER=0
            JUCE_USE_CURL=0
            JUCE_VST3_CAN_REPLACE_VST2=0
)