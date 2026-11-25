#include "PluginProcessor.h"
#include "PluginEditor.h"

CalculatorImbaAudioProcessor::CalculatorImbaAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

CalculatorImbaAudioProcessor::~CalculatorImbaAudioProcessor()
{
}

const juce::String CalculatorImbaAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CalculatorImbaAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CalculatorImbaAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CalculatorImbaAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CalculatorImbaAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CalculatorImbaAudioProcessor::getNumPrograms()
{
    return 1;
}

int CalculatorImbaAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CalculatorImbaAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String CalculatorImbaAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void CalculatorImbaAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index);
    juce::ignoreUnused (newName);
}

void CalculatorImbaAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void CalculatorImbaAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CalculatorImbaAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CalculatorImbaAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
}

bool CalculatorImbaAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* CalculatorImbaAudioProcessor::createEditor()
{
    return new CalculatorImbaAudioProcessorEditor (*this);
}

void CalculatorImbaAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ignoreUnused (destData);
}

void CalculatorImbaAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ignoreUnused (data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CalculatorImbaAudioProcessor();
}
