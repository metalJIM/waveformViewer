/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WaveformViewerAudioProcessor::WaveformViewerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), waveViewer(2)
#endif
{
    waveViewer.setRepaintRate(1024);
    //Higher the buffer size the more you'll see in the waveviewer at once
    waveViewer.setBufferSize(512);
}

WaveformViewerAudioProcessor::~WaveformViewerAudioProcessor()
{
}

//==============================================================================
const juce::String WaveformViewerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WaveformViewerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WaveformViewerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WaveformViewerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WaveformViewerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WaveformViewerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WaveformViewerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WaveformViewerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WaveformViewerAudioProcessor::getProgramName (int index)
{
    return {};
}

void WaveformViewerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WaveformViewerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    waveViewer.clear();
}

void WaveformViewerAudioProcessor::releaseResources()
{
    waveViewer.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WaveformViewerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void WaveformViewerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

   
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

   
    waveViewer.pushBuffer(buffer);
}

//==============================================================================
bool WaveformViewerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* WaveformViewerAudioProcessor::createEditor()
{
    return new WaveformViewerAudioProcessorEditor (*this);
}

//==============================================================================
void WaveformViewerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WaveformViewerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WaveformViewerAudioProcessor();
}
