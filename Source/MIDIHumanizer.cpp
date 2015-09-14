#include <ctime>

#include "MIDIHumanizer.h"

MIDIHumanizer::MIDIHumanizer()
{
    mMersenneTwisterTiming.seed ((unsigned int) std::time (0));
    mMersenneTwisterVelocity.seed (unsigned int (std::time (0)) + 4826);
}

MIDIHumanizer::~MIDIHumanizer()
{}

void MIDIHumanizer::processMIDIBuffer (MidiBuffer& inputMIDIBuffer)
{
    MidiBuffer::Iterator inputMIDIBufferIterator (inputMIDIBuffer);

    MidiMessage currentMidiMessage;
    int midiMessageSamplePosition = 0;

    if (! inputMIDIBuffer.isEmpty())
    {
        inputMIDIBufferIterator.getNextEvent (currentMidiMessage, midiMessageSamplePosition);
        bool midiBufferIsNotEmpty = true;

        for (int sampleIndex = 0; sampleIndex < mBlockSize; ++sampleIndex)
        {
            // Go through every MIDI message this sample.
            while (sampleIndex == midiMessageSamplePosition
                && midiBufferIsNotEmpty)
            {
                if (currentMidiMessage.isNoteOn())
                {                 
                    /*double randomOffset = generateNormalRandomNumber(mMersenneTwisterTiming) * mTimingStandardDeviationInSamples[sampleIndex];
                    double newSampleOffset = randomOffset + mMaximumDelayTimeInSamples;*/

                    double randomOffset = generateNormalRandomNumber(mMersenneTwisterTiming) * 662.0;
                    double newSampleOffset = randomOffset + mMaximumDelayTimeInSamples;

                    mSampleOffsetBuffer[currentMidiMessage.getNoteNumber()] = newSampleOffset;

                    double newVelocity = currentMidiMessage.getFloatVelocity();

                    /*double newVelocity = currentMidiMessage.getFloatVelocity() 
                                       + generateNormalRandomNumber(mMersenneTwisterVelocity) * mVelocityStandardDeviation[sampleIndex] / 127.0;*/

                    if (newVelocity > 1.0)
                    {
                        newVelocity = 1.0;
                    }

                    if (newVelocity <= 0.007)
                    {
                        newVelocity = 0.0078125;
                    }

                    currentMidiMessage.setVelocity (float (newVelocity));

                    TaggedMIDIMessage noteOnMessage (currentMidiMessage, 
                                                     int (midiMessageSamplePosition 
                                                   + mSampleOffsetBuffer[currentMidiMessage.getNoteNumber()]),
                                                     false);
                    mUnboundMIDIBuffer.addNoteOn (noteOnMessage);
                }

                if (currentMidiMessage.isNoteOff())
                {
                    double newVelocity = currentMidiMessage.getFloatVelocity();

                    /*double newVelocity = currentMidiMessage.getFloatVelocity() 
                                       + generateNormalRandomNumber(mMersenneTwisterVelocity) * mVelocityStandardDeviation[sampleIndex] / 127.0;*/

                    if (newVelocity > 1.0)
                    {
                        newVelocity = 1.0;
                    }

                    if (newVelocity <= 0.007)
                    {
                        newVelocity = 0.0078125;
                    }

                    currentMidiMessage.setVelocity (float (newVelocity));

                    TaggedMIDIMessage noteOffMessage (currentMidiMessage, 
                                                      int (midiMessageSamplePosition 
                                                    + mSampleOffsetBuffer[currentMidiMessage.getNoteNumber()]),
                                                      false);

                    mUnboundMIDIBuffer.addNoteOff (noteOffMessage);
                }
                
                if ((! currentMidiMessage.isNoteOn())
                 && (! currentMidiMessage.isNoteOff()))
                {
                    TaggedMIDIMessage currentTaggedMessage (currentMidiMessage,
                                                            int (midiMessageSamplePosition 
                                                          + mMaximumDelayTimeInSamples),
                                                            false);

                    mOtherMIDIExtendedBuffer.push_back (currentTaggedMessage);
                }

                midiBufferIsNotEmpty = inputMIDIBufferIterator.getNextEvent (currentMidiMessage, midiMessageSamplePosition);
            }
        }
    }

    if (! mOtherMIDIExtendedBuffer.empty())
    {
        for (int index = 0; index < mOtherMIDIExtendedBuffer.size(); ++index)
        {
            if ((mOtherMIDIExtendedBuffer[index].samplePosition < mBlockSize)
             && (mOtherMIDIExtendedBuffer[index].samplePosition >= 0))
            {
                mOtherMIDIEvents.addEvent (mOtherMIDIExtendedBuffer[index].message,
                                           mOtherMIDIExtendedBuffer[index].samplePosition);

                mOtherMIDIExtendedBuffer[index].samplePosition = -1;
            }
 
            if (mOtherMIDIExtendedBuffer[index].samplePosition >= mBlockSize)
            {
                mOtherMIDIExtendedBuffer[index].samplePosition -= mBlockSize;
            }

            if (mOtherMIDIExtendedBuffer[index].samplePosition < 0)
            {
                mOtherMIDIExtendedBuffer.erase (mOtherMIDIExtendedBuffer.begin() + index);
                --index;
            }   
        }
    }

    if (! mUnboundMIDIBuffer.isEmpty())
    {
        mUnboundMIDIBuffer.fixNoteOverlaps();

        for (int index = 0; index < mUnboundMIDIBuffer.getSize(); ++index)
        {
            pushMessageFromBuffer (mUnboundMIDIBuffer[index].noteOn);
            pushMessageFromBuffer (mUnboundMIDIBuffer[index].noteOff);
            
            if (((mUnboundMIDIBuffer[index].noteOn.samplePosition < 0)
             && ! mUnboundMIDIBuffer[index].noteOn.isPlaceholder)
                &&
                ((mUnboundMIDIBuffer[index].noteOff.samplePosition < 0)
             && ! mUnboundMIDIBuffer[index].noteOff.isPlaceholder))
            {
                mUnboundMIDIBuffer.removeCompleteMessage (index);
                --index;
            }         
        }
    }

    MidiBuffer::Iterator otherEventsIterator (mOtherMIDIEvents);     
    MidiMessage currentOtherEvent;
    int otherEventsSamplePosition = 0;

    while (otherEventsIterator.getNextEvent (currentOtherEvent, otherEventsSamplePosition))
    {
        mHumanizedMIDIBuffer.addEvent (currentOtherEvent, otherEventsSamplePosition);
    }

    inputMIDIBuffer.swapWith (mHumanizedMIDIBuffer);
    mHumanizedMIDIBuffer.clear();

    mOtherMIDIEvents.clear();
}

// Generates a normally distributed random number with a
// mean of 0.0, standard deviation of 1.0, and range of 
// ~ -2.933 to 2.933.
double MIDIHumanizer::generateNormalRandomNumber (boost::mt19937& inputRNG)
{ 
    boost::normal_distribution<> normalDistribution (0.0, 1.0);

    return (double) normalDistribution (inputRNG);
}

void MIDIHumanizer::pushMessageFromBuffer (TaggedMIDIMessage& inputMessage)
{
    if (! inputMessage.isPlaceholder)
    {
        if ((inputMessage.samplePosition < mBlockSize)
         && (inputMessage.samplePosition >= 0))
        {
            mHumanizedMIDIBuffer.addEvent (inputMessage.message,
                                           inputMessage.samplePosition);

            inputMessage.samplePosition = -1;
        }

        if (inputMessage.samplePosition >= mBlockSize)
        {
            inputMessage.samplePosition -= mBlockSize;
        }
    }
}

void MIDIHumanizer::setTimingStandardDeviation (std::vector<float> inputVector)   
{ 
    std::vector<float> bufferInSamples (inputVector);

    for (int index = 0; index < inputVector.size(); ++index)
    {
        float currentValue = inputVector[index];
        float valueInSamples = float (mSampleRate * currentValue / 1000.0);
        bufferInSamples[index] = valueInSamples;
    }

    mTimingStandardDeviationInSamples = bufferInSamples; 
};

void MIDIHumanizer::reset (double inputSampleRate, int inputBlockSize) 
{ 
    mSampleRate = inputSampleRate;
    mBlockSize = inputBlockSize; 

    mTimingStandardDeviationInSamples.resize (mBlockSize);
    mVelocityStandardDeviation.resize (mBlockSize);
};