#include <ctime>

#include "PluginEditor.h"
#include "FloatParameter.h"

#include "PluginProcessor.h"

//==============================================================================
AlkamistMIDIHumanizerAudioProcessor::AlkamistMIDIHumanizerAudioProcessor()
    : mParameterChangeFlag (false)
{
    double sampleRate = getSampleRate();
    int samplesPerBlock = getBlockSize();

    addParameter (timingStandardDeviation  = new FloatParameter (this, 0.0f, 0.0f, 100.0f, "Timing Standard Deviation", "ms", sampleRate, samplesPerBlock));

    for (int index = 0; index < taggedMIDIBufferSize; ++index)
    {
        taggedMIDIBuffer[index].samplePosition = 0;
        taggedMIDIBuffer[index].slotIsUsed = false;
    }

    mMersenneTwisterGenerator.seed (std::time (0));

    reset();
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

// Generates an approximately normally distributed random number with a
// standard deviation of 1 and a range of ~ -2.933 to 2.933.
double AlkamistMIDIHumanizerAudioProcessor::generateNormalRandomNumber()
{ 
    boost::normal_distribution<> normalDistribution (0.0, 1.0);

    return normalDistribution (mMersenneTwisterGenerator);
}

void AlkamistMIDIHumanizerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    setLatencySamples (mMaximumDelayTimeInSamples);

    MidiBuffer::Iterator MIDIMessagesIterator (midiMessages);
    MidiMessage defaultMIDIMessage;
    MidiMessage currentMidiMessage;
    int midiMessageSamplePosition = 0;

    if (! midiMessages.isEmpty())
    {
        MIDIMessagesIterator.getNextEvent (currentMidiMessage, midiMessageSamplePosition);
    }

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        if (! midiMessages.isEmpty())
        {
            bool midiBufferIsNotEmpty = true;

            // Go through every MIDI message this sample.
            while (sample == midiMessageSamplePosition
                   && midiBufferIsNotEmpty)
            {
                if (currentMidiMessage.isNoteOn() || currentMidiMessage.isNoteOff())
                {
                    bool searchForSlot = true;
                    int index = 0;
                    // Search for an open slot.
                    while (searchForSlot)
                    {
                        // If you find an open slot then write to it.
                        if (! taggedMIDIBuffer[index].slotIsUsed)
                        {
                            taggedMIDIBuffer[index].MIDIMessage = currentMidiMessage;
                            taggedMIDIBuffer[index].slotIsUsed = true;

                            if (currentMidiMessage.isNoteOn())
                            {
                                mMIDISampleOffsetBuffer[currentMidiMessage.getNoteNumber()] = generateNormalRandomNumber() * mTimingStandardDeviationInSamples;
                            }

                            taggedMIDIBuffer[index].samplePosition = mMIDISampleOffsetBuffer[currentMidiMessage.getNoteNumber()]
                                                                   + midiMessageSamplePosition
                                                                   + mMaximumDelayTimeInSamples;

                            if (taggedMIDIBuffer[index].samplePosition < 0)
                            {
                                taggedMIDIBuffer[index].samplePosition = 0;
                            }

                            searchForSlot = false;
                        }

                        if (index > taggedMIDIBufferSize)
                        {
                            searchForSlot = false;
                        }

                        ++index;
                    }
                }

                midiBufferIsNotEmpty = MIDIMessagesIterator.getNextEvent (currentMidiMessage, midiMessageSamplePosition);
            }
        }

        if (mParameterChangeFlag == true)
        {
            handleParameterChanges();
        }
    }

    for (int index = 0; index < taggedMIDIBufferSize; ++index)
    {
        if (taggedMIDIBuffer[index].samplePosition < getBlockSize()
            && taggedMIDIBuffer[index].slotIsUsed)
        {
            humanizedMIDIBuffer.addEvent (taggedMIDIBuffer[index].MIDIMessage, 
                                          taggedMIDIBuffer[index].samplePosition);
                                          
            taggedMIDIBuffer[index].MIDIMessage = defaultMIDIMessage;
            taggedMIDIBuffer[index].samplePosition = 0;
            taggedMIDIBuffer[index].slotIsUsed = false;
        }
        
        if (taggedMIDIBuffer[index].samplePosition > getBlockSize()
            && taggedMIDIBuffer[index].slotIsUsed)
        {
            taggedMIDIBuffer[index].samplePosition -= getBlockSize();
        }
    }

    midiMessages.swapWith (humanizedMIDIBuffer);
    humanizedMIDIBuffer.clear();

    clearParameterChanges();
    mParameterChangeFlag = false;
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
        }
    }
}

//==============================================================================
void AlkamistMIDIHumanizerAudioProcessor::clearParameterChanges()
{
    timingStandardDeviation->clearParameterChangeFlag();
}

void AlkamistMIDIHumanizerAudioProcessor::handleParameterChanges()
{
    if (timingStandardDeviation->needsToChange())  
    {
        timingStandardDeviation->processPerSample();

        mTimingStandardDeviationInSamples = getSampleRate() * timingStandardDeviation->getUnNormalizedSmoothedValue() / 1000.0;
        mMaximumDelayTimeInSamples = mTimingStandardDeviationInSamples / 0.341;
    }
}

void AlkamistMIDIHumanizerAudioProcessor::reset()
{
    double sampleRate = getSampleRate();
    int samplesPerBlock = getBlockSize();

    // Parameters
    timingStandardDeviation->reset (sampleRate, samplesPerBlock);
}
//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AlkamistMIDIHumanizerAudioProcessor();
}





