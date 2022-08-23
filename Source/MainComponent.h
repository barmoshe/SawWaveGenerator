#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 This component lives inside our window, and this is where you should put all
 your controls and content.
 */
class MainComponent  : public juce::AudioAppComponent,
public juce::Button::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;
    
    void extracted();
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    void buttonClicked (juce::Button*) override;
private:
    //==============================================================================
    juce::Random rand;
    juce::Array<float> sawWaveTable;
    double waveTableSize;
    double freq;
    double phase;
    double phaseIncrement;
    double amp;
    double sampleRate1;
    int isOn;
    juce::Slider ampSlider;
    juce::Label ampLabel;
    juce::Slider freqSlider;
    juce::Label freqLabel;
    juce::TextButton btn;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
