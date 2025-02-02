cmake_minimum_required(VERSION 3.22)
project(AudioPlugin VERSION 0.1.0)
juce_add_plugin(${PROJECT_NAME}
        COMPANY_NAME gaucho dsp
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        PLUGIN_MANUFACTURER_CODE GDSP
        PLUGIN_CODE AUPL
        FORMATS VST3
        PRODUCT_NAME "ushuaiaVerb"
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
if(MSVC)
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        _SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING
)
endif()
