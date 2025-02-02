//
// Created by Cucu on 02/02/2025.
//
#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED 1
#include "juce_core/juce_core.h"
#include <gtest/gtest.h>
#include <ushuaiaVerb/PluginProcessor.h>
namespace audio_plugin_test {
TEST(AudioPluginAudioProcessor, Foo) {
  AudioPluginAudioProcessor processor{};
  ASSERT_FALSE(true);
}
}