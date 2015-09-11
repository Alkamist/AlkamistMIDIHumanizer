#include <ctime>

#include "MIDIHumanizer.h"

MIDIHumanizer::MIDIHumanizer()
{
    mMersenneTwisterRNG.seed ((unsigned int) std::time (0));
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
                    double randomOffset = generateNormalRandomNumber() * mTimingStandardDeviationInSamples;
                    double newSampleOffset = randomOffset + mMaximumDelayTimeInSamples;

                    mSampleOffsetBuffer[currentMidiMessage.getNoteNumber()] = newSampleOffset;

                    TaggedMIDIMessage noteOnMessage (currentMidiMessage, 
                                                     int (midiMessageSamplePosition 
                                                   + mSampleOffsetBuffer[currentMidiMessage.getNoteNumber()]),
                                                     false);
                    mUnboundMIDIBuffer.addNoteOn (noteOnMessage);
                }

                if (currentMidiMessage.isNoteOff())
                {
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

            //parameterChangeSignal();
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
double MIDIHumanizer::generateNormalRandomNumber()
{ 
    boost::normal_distribution<> normalDistribution (0.0, 1.0);

    return (double) normalDistribution (mMersenneTwisterRNG);
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