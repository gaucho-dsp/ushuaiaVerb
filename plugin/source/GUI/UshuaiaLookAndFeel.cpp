//
// Adapted from Maim's LookAndFeel
//

#include "ushuaiaVerb/GUI/UshuaiaLookAndFeel.h"

UshuaiaLookAndFeel::UshuaiaLookAndFeel() 
{

    setColour(juce::ResizableWindow::backgroundColourId, UshuaiaColours::PANEL_BACKGROUND_COLOR);
    setColour(juce::ComboBox::backgroundColourId, UshuaiaColours::BEVEL_LIGHT);
    setColour(juce::ComboBox::textColourId, UshuaiaColours::BEVEL_BLACK);
    setColour(juce::ComboBox::outlineColourId, UshuaiaColours::BEVEL_DARK);
    setColour(juce::ComboBox::focusedOutlineColourId, UshuaiaColours::BEVEL_BLACK);
    setColour(juce::ComboBox::arrowColourId, UshuaiaColours::BEVEL_DARK);

    setColour(juce::PopupMenu::backgroundColourId, UshuaiaColours::BEVEL_LIGHT);
    setColour(juce::PopupMenu::textColourId, UshuaiaColours::BEVEL_DARK);
    setColour(juce::PopupMenu::headerTextColourId, UshuaiaColours::BEVEL_BLACK);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, UshuaiaColours::BEVEL_WHITE);
    setColour(juce::PopupMenu::highlightedTextColourId, UshuaiaColours::BEVEL_BLACK);

    setColour(juce::Slider::textBoxTextColourId, UshuaiaColours::BEVEL_DARK);
    const float transparent_alpha = 0;
    setColour(juce::Slider::textBoxOutlineColourId,
        UshuaiaColours::PANEL_BACKGROUND_COLOR.withAlpha(transparent_alpha));
    setColour(juce::Slider::textBoxHighlightColourId, UshuaiaColours::BEVEL_BLACK);
    setColour(juce::Slider::textBoxBackgroundColourId,
        UshuaiaColours::PANEL_BACKGROUND_COLOR.withAlpha(transparent_alpha));
    setColour(juce::Label::textWhenEditingColourId, UshuaiaColours::BEVEL_BLACK);

    setColour(juce::CaretComponent::caretColourId, juce::Colour(0xff2e2edb));
}

UshuaiaLookAndFeel::~UshuaiaLookAndFeel()
{
}

void UshuaiaLookAndFeel::drawRotarySlider (juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPos,
    const float rotaryStartAngle,
    const float rotaryEndAngle,
    juce::Slider& slider)
{

    juce::Colour fill = juce::Colours::white;
    juce::Colour outline = juce::Colours::black;
    juce::Colour notch = juce::Colour(0xff2d2d46);
    bool focused = false;

    const auto radius = (float) juce::jmin(width / 2, height / 2) - 5;
    const auto inner_radius = focused ? radius - 2 : radius;
    const auto centerx = (float) x + (float) width  * 0.5f;
    const auto centery = (float) y + (float) height * 0.5f;

    g.setColour(fill);
    g.fillEllipse(centerx - inner_radius,
        centery - inner_radius,
        inner_radius * 2,
        inner_radius * 2);

    g.setColour(outline);
    g.drawEllipse(centerx - radius,centery - radius,radius*2,radius*2, 1);

    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    g.setColour (notch);

    juce::Path p;
    auto pointerLength = inner_radius;
    auto pointerThickness = 2.0f;
    p.addRectangle (-pointerThickness * 0.5f, -inner_radius, pointerThickness, pointerLength);
    p.applyTransform (juce::AffineTransform::rotation (angle).translated (centerx, centery));
    g.fillPath (p);
}

void UshuaiaLookAndFeel::drawLinearSlider(juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPos,
    float minSliderPos,
    float maxSliderPos,
    const juce::Slider::SliderStyle style,
    juce::Slider& slider)
{
    if (style == juce::Slider::LinearHorizontal || style == juce::Slider::LinearVertical)
    {
        const bool isHorizontal = style == juce::Slider::LinearHorizontal;
        const float trackWidth = isHorizontal ? height * 0.25f : width * 0.25f;
        
        juce::Rectangle<float> track;
        if (isHorizontal)
            track.setBounds(x, y + height * 0.5f - trackWidth * 0.5f, width, trackWidth);
        else
            track.setBounds(x + width * 0.5f - trackWidth * 0.5f, y, trackWidth, height);

        // Draw track background
        g.setColour(UshuaiaColours::BEVEL_BLACK);
        g.fillRect(track);
        
        g.setColour(UshuaiaColours::BEVEL_DARK);
        g.fillRect(track.reduced(1));

        // Draw thumb
        float thumbWidth = 14.0f;
        float thumbHeight = 18.0f;
        
        juce::Rectangle<float> thumb;
        if (isHorizontal)
            thumb.setBounds(sliderPos - thumbWidth * 0.5f, y + height * 0.5f - thumbHeight * 0.5f, thumbWidth, thumbHeight);
        else
            thumb.setBounds(x + width * 0.5f - thumbWidth * 0.5f, sliderPos - thumbHeight * 0.5f, thumbWidth, thumbHeight);

        g.setColour(UshuaiaColours::BEVEL_LIGHT);
        g.fillRect(thumb);
        
        g.setColour(UshuaiaColours::BEVEL_BLACK);
        g.drawRect(thumb, 1);
        
        // Draw center line on thumb
        g.setColour(slider.isEnabled() ? UshuaiaColours::SPLASH_COLOR_DARK : UshuaiaColours::BEVEL_DARK);
        if (isHorizontal)
            g.fillRect(static_cast<int>(thumb.getCentreX() - 1), static_cast<int>(thumb.getY() + 2), 2, static_cast<int>(thumb.getHeight() - 4));
        else
            g.fillRect(static_cast<int>(thumb.getX() + 2), static_cast<int>(thumb.getCentreY() - 1), static_cast<int>(thumb.getWidth() - 4), 2);
    }
}

void UshuaiaLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, 
                                          bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds();
    auto size = juce::jmin(20, bounds.getHeight() - 4);
    auto buttonRect = bounds.removeFromLeft(size).withSizeKeepingCentre(size, size).toFloat();

    // Draw outer border
    g.setColour(UshuaiaColours::BEVEL_BLACK);
    g.drawRect(buttonRect, 1.0f);

    // Draw inner background
    g.setColour(button.getToggleState() ? UshuaiaColours::SPLASH_COLOR_LIGHT : UshuaiaColours::BEVEL_LIGHT);
    g.fillRect(buttonRect.reduced(2));

    // Draw check mark if toggled
    if (button.getToggleState())
    {
        juce::Path checkMark;
        checkMark.startNewSubPath(buttonRect.getX() + size * 0.2f, buttonRect.getY() + size * 0.5f);
        checkMark.lineTo(buttonRect.getX() + size * 0.4f, buttonRect.getY() + size * 0.7f);
        checkMark.lineTo(buttonRect.getX() + size * 0.8f, buttonRect.getY() + size * 0.3f);
        
        g.setColour(UshuaiaColours::BEVEL_BLACK);
        g.strokePath(checkMark, juce::PathStrokeType(2.0f));
    }

    // Draw text
    g.setColour(button.isEnabled() ? UshuaiaColours::TEXT_COLOR_PRIMARY : UshuaiaColours::TEXT_COLOR_DISABLED);
    g.setFont(13.0f);
    g.drawText(button.getButtonText(), bounds.reduced(4), juce::Justification::centredLeft);
}

