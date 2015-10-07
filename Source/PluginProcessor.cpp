#include "PluginEditor.h"
#include "FloatParameter.h"
#include "PluginProcessor.h"

const int PDC_DELAY_TIME = 1024;

//==============================================================================
AlkamistMIDIHumanizerAudioProcessor::AlkamistMIDIHumanizerAudioProcessor()
{
    double sampleRate = getSampleRate();
    int blockSize = getBlockSize();

    addParameter (timingRange  = new FloatParameter (0.0f, 0.0f, 20.0f, "Timing Range", "ms", sampleRate, blockSize));
    addParameter (velocityRange  = new FloatParameter (0.0f, 0.0f, 64.0f, "Velocity Range", "", sampleRate, blockSize));

    reset (sampleRate, blockSize);

    mMIDIHumanizer.setMaximumDelayTime (PDC_DELAY_TIME);

    setLatencySamples (PDC_DELAY_TIME);
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
void AlkamistMIDIHumanizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    reset (sampleRate, samplesPerBlock);
}

void AlkamistMIDIHumanizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void AlkamistMIDIHumanizerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    bufferParameters();
    sendParameterBuffers();

    mMIDIHumanizer.processMIDIBuffer (midiMessages);

    clearParameterChanges();

    for (int index = getNumInputChannels(); index < getNumOutputChannels(); ++index)
        buffer.clear (index, 0, buffer.getNumSamples());
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
    xmlPointer = xmlRoot.createNewChildElement ("timingRange");
    xmlPointer->addTextElement (String (timingRange->getUnNormalizedUnSmoothedValue()));

    xmlPointer = xmlRoot.createNewChildElement ("velocityRange");
    xmlPointer->addTextElement (String (velocityRange->getUnNormalizedUnSmoothedValue()));

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
            if(xmlChildPointer->hasTagName("timingRange"))
            {
                String text = xmlChildPointer->getAllSubText();
                timingRange->setNormalizedValue (text.getFloatValue());
            }

            if(xmlChildPointer->hasTagName("velocityRange"))
            {
                String text = xmlChildPointer->getAllSubText();
                velocityRange->setNormalizedValue (text.getFloatValue());
            }
        }
    }
}

//==============================================================================

void AlkamistMIDIHumanizerAudioProcessor::bufferParameters()
{
    timingRange->bufferParameter();
    velocityRange->bufferParameter();
}

void AlkamistMIDIHumanizerAudioProcessor::sendParameterBuffers()
{
    if (timingRange->parameterChangedThisBlock()
        || timingRange->parameterNeedsToSendFlatBuffer())
    {
        mMIDIHumanizer.setTimingRange (timingRange->getUnNormalizedSmoothedBuffer());
    }

    if (velocityRange->parameterChangedThisBlock()
        || velocityRange->parameterNeedsToSendFlatBuffer())
    {
        mMIDIHumanizer.setVelocityRange (velocityRange->getUnNormalizedSmoothedBuffer());
    }
}

void AlkamistMIDIHumanizerAudioProcessor::clearParameterChanges()
{
    timingRange->clearParameterChange();
    velocityRange->clearParameterChange();
}

void AlkamistMIDIHumanizerAudioProcessor::reset (double inputSampleRate, int inputBlockSize)
{
    mMIDIHumanizer.reset (inputSampleRate, inputBlockSize);

    // Parameters
    timingRange->reset (inputSampleRate, inputBlockSize);
    velocityRange->reset (inputSampleRate, inputBlockSize);

    timingRange->signalForParameterChange();
    velocityRange->signalForParameterChange();
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AlkamistMIDIHumanizerAudioProcessor();
}





