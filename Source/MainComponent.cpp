#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (700, 200);
    
    addAndMakeVisible(btn);
    btn.setButtonText("PLAY");
    btn.setClickingTogglesState(true);
    btn.setColour(juce::TextButton::ColourIds::buttonOnColourId, juce::Colours::lime);
    btn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkgreen);
    btn.addListener(this);
    
    addAndMakeVisible(freqSlider);
    freqSlider.setRange(50, 3000);
    freqSlider.setTextValueSuffix("Hz");
    freqSlider.setValue(440);
    
    addAndMakeVisible(freqLabel);
    freqLabel.setText("Frequency", juce::NotificationType::dontSendNotification);
    freqLabel.attachToComponent(&freqSlider, true);
    freqSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 100, 20);
    
    addAndMakeVisible(ampSlider);
    ampSlider.setRange(0, 100);
    ampSlider.setTextValueSuffix("%");
    
    addAndMakeVisible(ampLabel);
    ampLabel.setText("Amplitude", juce::NotificationType::dontSendNotification);
    ampLabel.attachToComponent(&ampSlider, true);
    ampSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 100, 20);
    
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    isOn=0;
    freq=freqSlider.getValue();
    phase =0;
    waveTableSize=8192;
    amp=0.25;
    sampleRate1=sampleRate;
    phaseIncrement=freq*waveTableSize/sampleRate;
    
    //calc one SAW cycle
    for (int i=0; i<waveTableSize; i++) {
        if(i<=waveTableSize/2)
        {
            sawWaveTable.insert(i, i/(waveTableSize/2));
        }else
        {
            sawWaveTable.insert(i, ((i-waveTableSize/2)/(waveTableSize/2))-1);
        }
        
    }
}



void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    float* const leftSpeaker=bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    float* const rightSpeaker=bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
    amp=ampSlider.getValue()/100;
    freq=freqSlider.getValue();
    phaseIncrement=freq*waveTableSize/sampleRate1;
    for (int sample=0; sample<bufferToFill.numSamples; sample++) {
        leftSpeaker[sample]=sawWaveTable[(int)phase]*amp*isOn;
        rightSpeaker[sample]=sawWaveTable[(int)phase]*amp*isOn;
        phase=fmod(phase+phaseIncrement,waveTableSize);
        
    }
}
void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.
    
    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    btn.setBounds( getLocalBounds().getWidth()/2-getLocalBounds().getWidth()/24, 10, getLocalBounds().getWidth()/12, getLocalBounds().getWidth()/12);
    juce::Rectangle<int> volumeSliderBounds=getLocalBounds();
    volumeSliderBounds.setY((getLocalBounds().getHeight()/1.5));
    volumeSliderBounds.setWidth(getLocalBounds().getWidth()/3);
    volumeSliderBounds.setX(getLocalBounds().getWidth()/2-(ampSlider.getWidth()/2)+ampLabel.getWidth()/2);
    volumeSliderBounds.setHeight(getLocalBounds().getHeight()/8);
    ampSlider.setBounds(volumeSliderBounds);
    juce::Rectangle<int> freqSliderBounds=getLocalBounds();
    freqSliderBounds.setY((getLocalBounds().getHeight()/1.5)-(getLocalBounds().getHeight()/10));
    freqSliderBounds.setWidth((getLocalBounds().getWidth()/3));
    freqSliderBounds.setX(getLocalBounds().getWidth()/2-(freqSlider.getWidth()/2)+freqLabel.getWidth()/2);
    freqSliderBounds.setHeight(getLocalBounds().getHeight()/8);
    
    freqSlider.setBounds(freqSliderBounds);
    g.fillAll (juce::Colours::royalblue);
    g.setColour (juce::Colours::white);
    g.setFont (20.0f);
    g.drawText ("Saw Wave Sound!!", getLocalBounds(), juce::Justification::centred, true);
    
    
}
void MainComponent::buttonClicked( juce::Button* b)
{
    switch (isOn) {
        case 0:
            isOn=1;
            btn.setButtonText("PAUSE");
            
            break;
        case 1:
            isOn=0;
            btn.setButtonText("PLAY");
            
            break;
            
            
    }
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
