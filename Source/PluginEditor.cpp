#include "PluginProcessor.h"
#include "PluginEditor.h"

CalculatorImbaAudioProcessorEditor::CalculatorImbaAudioProcessorEditor (CalculatorImbaAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    backgroundImage = juce::ImageCache::getFromMemory (BinaryData::bg_jpg, BinaryData::bg_jpgSize);

    titleLabel.setText ("CALCULATOR IMBA))", juce::dontSendNotification);
    titleLabel.setFont (juce::FontOptions (12.0f, juce::Font::bold));
    titleLabel.setJustificationType (juce::Justification::centredLeft);
    titleLabel.setColour (juce::Label::textColourId, juce::Colour (0xffff9500));
    addAndMakeVisible (titleLabel);

    display.setText ("0", juce::dontSendNotification);
    display.setFont (juce::FontOptions (32.0f, juce::Font::bold));
    display.setJustificationType (juce::Justification::centredRight);
    display.setColour (juce::Label::backgroundColourId, juce::Colour (0xcc1a1a1d));
    display.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (display);

    const juce::StringArray labels = {
        "C", "/", "*", "-",
        "7", "8", "9", "+",
        "4", "5", "6", "=",
        "1", "2", "3", "0", "."
    };

    for (const auto& text : labels)
    {
        auto* btn = buttons.add (new juce::TextButton (text));
        btn->setColour (juce::TextButton::buttonColourId,
                        (text == "=" || text == "+" || text == "-" || text == "*" || text == "/")
                        ? juce::Colour (0xd9ff9500)
                        : (text == "C" ? juce::Colour (0xd9a5a5a5) : juce::Colour (0xcc252528)));
        btn->setColour (juce::TextButton::textColourOffId,
                        text == "C" ? juce::Colours::black : juce::Colours::white);

        btn->onClick = [this, text] { onButtonClicked (text); };
        addAndMakeVisible (btn);
    }

    setSize (280, 400);
}

CalculatorImbaAudioProcessorEditor::~CalculatorImbaAudioProcessorEditor() {}

void CalculatorImbaAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff121214));

    if (backgroundImage.isValid())
    {
        g.drawImage (backgroundImage, getLocalBounds().toFloat(),
                     juce::RectanglePlacement::fillDestination);
        g.setColour (juce::Colours::black.withAlpha (0.25f));
        g.fillAll();
    }
}

void CalculatorImbaAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (10);
    titleLabel.setBounds (area.removeFromTop (18));
    display.setBounds (area.removeFromTop (68).reduced (2));
    area.removeFromTop (8);

    const int cols = 4;
    const int rows = 5;
    int btnW = area.getWidth() / cols;
    int btnH = area.getHeight() / rows;

    const char* grid = {
        { "C", "/", "*", "-" },
        { "7", "8", "9", "+" },
        { "4", "5", "6", "=" },
        { "1", "2", "3", ""  },
        { "0", ".", "",  ""  }
    };

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            juce::String target (grid[r][c]);
            if (target.isEmpty()) continue;

            for (auto* btn : buttons)
            {
                if (btn->getButtonText() == target)
                {
                    int w = (target == "0") ? btnW * 2 : btnW;
                    int h = (target == "+" || target == "=") ? btnH * 2 : btnH;
                    btn->setBounds (area.getX() + c * btnW + 2,
                                    area.getY() + r * btnH + 2,
                                    w - 4, h - 4);
                    break;
                }
            }
        }
    }
}

void CalculatorImbaAudioProcessorEditor::onButtonClicked (const juce::String& label)
{
    auto currentText = display.getText();

    if (label == "C")
    {
        display.setText ("0", juce::dontSendNotification);
        firstNumber = 0.0;
        pendingOperator.clear();
        waitingForSecondNumber = false;
        return;
    }

    if (label == "+" || label == "-" || label == "*" || label == "/")
    {
        firstNumber = currentText.getDoubleValue();
        pendingOperator = label;
        waitingForSecondNumber = true;
        return;
    }

    if (label == "=")
    {
        if (pendingOperator.isNotEmpty())
        {
            double secondNumber = currentText.getDoubleValue();
            double result = calculate (firstNumber, secondNumber, pendingOperator);
            display.setText (juce::String (result), juce::dontSendNotification);
            pendingOperator.clear();
            waitingForSecondNumber = true;
        }
        return;
    }

    if (waitingForSecondNumber)
    {
        currentText = (label == ".") ? "0." : label;
        waitingForSecondNumber = false;
    }
    else
    {
        if (label == "." && currentText.contains ("."))
            return;
        currentText = (currentText == "0" && label != ".") ? label : currentText + label;
    }

    display.setText (currentText, juce::dontSendNotification);
}

double CalculatorImbaAudioProcessorEditor::calculate (double a, double b, const juce::String& op)
{
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return (b != 0.0) ? (a / b) : 0.0;
    return b;
}
