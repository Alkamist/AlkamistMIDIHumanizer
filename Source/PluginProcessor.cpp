#include "PluginEditor.h"
#include "FloatParameter.h"

#include "PluginProcessor.h"

const int PDC_DELAY_TIME = 4096;

//==============================================================================
AlkamistMIDIHumanizerAudioProcessor::AlkamistMIDIHumanizerAudioProcessor()
    : mParameterChangeFlag (false)
{
    double sampleRate = getSampleRate();
    int samplesPerBlock = getBlockSize();

    addParameter (timingStandardDeviation  = new FloatParameter (this, 0.0f, 0.0f, 30.0f, "Timing Standard Deviation", "ms", sampleRate, samplesPerBlock));
    addParameter (velocityStandardDeviation  = new FloatParameter (this, 0.0f, 0.0f, 127.0f, "Velocity Standard Deviation", "", sampleRate, samplesPerBlock));

    reset();

    mMIDIHumanizer.setTimingStandardDeviationInSamples (getSampleRate() * timingStandardDeviation->getUnNormalizedUnSmoothedValue() / 1000.0);
    mMIDIHumanizer.setVelocityStandardDeviation (velocityStandardDeviation->getUnNormalizedUnSmoothedValue());
    mMIDIHumanizer.setMaximumDelayTimeInSamples (PDC_DELAY_TIME);

    setLatencySamples (PDC_DELAY_TIME);
    //mMIDIHumanizer.parameterChangeSignal.Connect (this, &AlkamistMIDIHumanizerAudioProcessor::handleParameterChanges);
}

AlkamistMIDIHumanizerAudioProcessor::~AlkamistMIDIHumanizerAudioProcessor()
{
}

//==============================================================================
const String AlkamistMIDIHumanizerAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String AlkamistMIDIHumanizerAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool AlkamistMIDIHumanizerAudioProcessor::isInputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool AlkamistMIDIHumanizerAudioProcessor::isOutputChannelStereoPair (int /*index*/) const
{
    return true;
}

bool AlkamistMIDIHumanizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AlkamistMIDIHumanizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AlkamistMIDIHumanizerAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double AlkamistMIDIHumanizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//==============================================================================
void AlkamistMIDIHumanizerAudioProcessor::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    reset();
}

void AlkamistMIDIHumanizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void AlkamistMIDIHumanizerAudioProcessor::processBlock (AudioSampleBuffer& /*buffer*/, MidiBuffer& midiMessages)
{
    if (mParameterChangeFlag == true)
    {
        handleParameterChanges();
    }

    mMIDIHumanizer.processMIDIBuffer (midiMessages);

    clearParameterChanges();
}

//==============================================================================
AudioProcessorEditor* AlkamistMIDIHumanizerAudioProcessor::createEditor()
{
    return new AlkamistMIDIHumanizerAudioProcessorEditor (*this);
}

//==============================================================================
void AlkamistMIDIHumanizerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    // Create an outer XML element..
    XmlElement xmlRoot ("ALKAMISTMIDIHUMANIZERSETTINGS");
    XmlElement* xmlPointer;

    // Document our parameter values in XML child elements.
    xmlPointer = xmlRoot.createNewChildElement ("timingStandardDeviation");
    xmlPointer->addTextElement (String (timingStandardDeviation->getValue()));

    xmlPointer = xmlRoot.createNewChildElement ("velocityStandardDeviation");
    xmlPointer->addTextElement (String (velocityStandardDeviation->getValue()));

    // Use this helper function to stuff it into the binary blob and return it.
    copyXmlToBinary (xmlRoot, destData);
}

void AlkamistMIDIHumanizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    ScopedPointer<XmlElement> xmlPluginState (getXmlFromBinary (data, sizeInBytes));

    if (xmlPluginState != nullptr)
    {
        forEachXmlChildElement (*xmlPluginState, xmlChildPointer)
        {
            if(xmlChildPointer->hasTagName("timingStandardDeviation"))
            {
                String text = xmlChildPointer->getAllSubText();
                timingStandardDeviation->setValue (text.getFloatValue());
            }

            if(xmlChildPointer->hasTagName("velocityStandardDeviation"))
            {
                String text = xmlChildPointer->getAllSubText();
                velocityStandardDeviation->setValue (text.getFloatValue());
            }
        }
    }
}

//==============================================================================
void AlkamistMIDIHumanizerAudioProcessor::clearParameterChanges()
{
    timingStandardDeviation->clearParameterChangeFlag();
    velocityStandardDeviation->clearParameterChangeFlag();

    mParameterChangeFlag = false;
}

void AlkamistMIDIHumanizerAudioProcessor::handleParameterChanges()
{
    /*if (timingStandardDeviation->needsToChange())  
    {
        timingStandardDeviation->processPerSample();
        double standardDeviation = getSampleRate() * timingStandardDeviation->getUnNormalizedSmoothedValue() / 1000.0;
        mMIDIHumanizer.setTimingStandardDeviationInSamples (standardDeviation);
        mMIDIHumanizer.setMaximumDelayTimeInSamples (standardDeviation / 0.341);
    }*/

    if (timingStandardDeviation->needsToChange())  
    {
        double standardDeviation = getSampleRate() * timingStandardDeviation->getUnNormalizedUnSmoothedValue() / 1000.0;
        mMIDIHumanizer.setTimingStandardDeviationInSamples (standardDeviation);
    }

    if (velocityStandardDeviation->needsToChange())  
    {
        double standardDeviation = velocityStandardDeviation->getUnNormalizedUnSmoothedValue();
        mMIDIHumanizer.setVelocityStandardDeviation (standardDeviation);
    }
}

void AlkamistMIDIHumanizerAudioProcessor::reset()
{
    double sampleRate = getSampleRate();
    int samplesPerBlock = getBlockSize();

    mMIDIHumanizer.reset (samplesPerBlock);

    // Parameters
    timingStandardDeviation->reset (sampleRate, samplesPerBlock);
    velocityStandardDeviation->reset (sampleRate, samplesPerBlock);
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AlkamistMIDIHumanizerAudioProcessor();
}





