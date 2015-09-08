#include <ctime>

#include "MIDIHumanizer.h"

MIDIHumanizer::MIDIHumanizer()
{
    mMersenneTwisterRNG.seed ((unsigned int) std::time (0));
}

MIDIHumanizer::~MIDIHumanizer()
{}

void MIDIHumanizer::processMIDIBuffer (MidiBuffer &inputMIDIBuffer)
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
                if (currentMidiMessage.isNoteOn() || currentMidiMessage.isNoteOff())
                {
                    /*int newSamplePosition = generateNormalRandomNumber() * mTimingStandardDeviationInSamples 
                                          + midiMessageSamplePosition 
                                          + mMaximumDelayTimeInSamples;*/
                    if (currentMidiMessage.isNoteOn())
                    {
                        sampleOffsetBuffer[currentMidiMessage.getNoteNumber()] = generateNormalRandomNumber() * 200;
                    }

                    int newSamplePosition = midiMessageSamplePosition
                                          + sampleOffsetBuffer[currentMidiMessage.getNoteNumber()]
                                          + 5000;

                    mUnboundMIDIBuffer.addMessage (currentMidiMessage, newSamplePosition);
                }

                midiBufferIsNotEmpty = inputMIDIBufferIterator.getNextEvent (currentMidiMessage, midiMessageSamplePosition);
            }
        }
    }

    if (! mUnboundMIDIBuffer.isEmpty())
    {
        //mUnboundMIDIBuffer.fixNoteOverlaps();

        for (int index = 0; index < mUnboundMIDIBuffer.getSize(); ++index)
        {
            if (mUnboundMIDIBuffer[index].samplePosition < mBlockSize)
            {
                mHumanizedMIDIBuffer.addEvent (mUnboundMIDIBuffer[index].message,
                                               mUnboundMIDIBuffer[index].samplePosition);

                mUnboundMIDIBuffer.eraseMessage (index);
            }
            else
            {
                mUnboundMIDIBuffer[index].samplePosition -= mBlockSize;
            }
        }

        inputMIDIBuffer.swapWith (mHumanizedMIDIBuffer);
        mHumanizedMIDIBuffer.clear();
    }
}

// Generates a normally distributed random number with a
// standard deviation of 1 and a range of ~ -2.933 to 2.933.
double MIDIHumanizer::generateNormalRandomNumber()
{ 
    boost::normal_distribution<> normalDistribution (0.0, 1.0);

    return (double) normalDistribution (mMersenneTwisterRNG);
}