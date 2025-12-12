//
// Created for UshuaiaVerb
// Title panel with plugin name and info
//

#ifndef USHUAIAVERB_TITLEPANEL_H
#define USHUAIAVERB_TITLEPANEL_H

#include "StageWindow.h"

class TitlePanel : public StageWindow
{
public:
    TitlePanel();
    ~TitlePanel();
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitlePanel)
};

#endif //USHUAIAVERB_TITLEPANEL_H