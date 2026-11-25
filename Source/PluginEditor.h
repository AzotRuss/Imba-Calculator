#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CalculatorImbaAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    CalculatorImbaAudioProcessorEditor (CalculatorImbaAudioProcessor&);
    ~CalculatorImbaAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CalculatorImbaAudioProcessor& audioProcessor;

    juce::Image backgroundImage;
    juce::Label titleLabel;
    juce::Label display;
    juce::OwnedArray<juce::TextButton> buttons;

    double firstNumber = 0.0;
    juce::String pendingOperator;
    bool waitingForSecondNumber = false;

    void onButtonClicked (const juce::String& label);
    double calculate (double a, double b, const juce::String& op);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CalculatorImbaAudioProcessorEditor)
};
