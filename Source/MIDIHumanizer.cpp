#include <ctime>

#include "MIDIHumanizer.h"

MIDIHumanizer::MIDIHumanizer()
{
    mMersenneTwisterRNG.seed (std::time (0));
}

MIDIHumanizer::~MIDIHumanizer()
{}

void MIDIHumanizer::processMIDIBuffer (MidiBuffer &inputMIDIBuffer, int blockSize)
{
    MidiBuffer::Iterator inputMIDIBufferIterator (inputMIDIBuffer);

    MidiMessage currentMidiMessage;
    int midiMessageSamplePosition = 0;

    if (! inputMIDIBuffer.isEmpty())
    {
        inputMIDIBufferIterator.getNextEvent (currentMidiMessage, midiMessageSamplePosition);
        bool midiBufferIsNotEmpty = true;

        for (int sampleIndex = 0; sampleIndex < blockSize; ++sampleIndex)
        {
            // Go through every MIDI message this sample.
            while (sampleIndex == midiMessageSamplePosition
                   && midiBufferIsNotEmpty)
            {
                if (currentMidiMessage.isNoteOn() || currentMidiMessage.isNoteOff())
                {
                    int newSamplePosition = generateNormalRandomNumber() * mTimingStandardDeviationInSamples 
                                          + midiMessageSamplePosition 
                                          + mMaximumDelayTimeInSamples;

                    mUnboundMIDIBuffer.addMessage (currentMidiMessage, newSamplePosition);
                }

                midiBufferIsNotEmpty = inputMIDIBufferIterator.getNextEvent (currentMidiMessage, midiMessageSamplePosition);
            }
        }
    }

    if (! mUnboundMIDIBuffer.isEmpty())
    {
    }
}

// Generates a normally distributed random number with a
// standard deviation of 1 and a range of ~ -2.933 to 2.933.
double MIDIHumanizer::generateNormalRandomNumber()
{ 
    boost::normal_distribution<> normalDistribution (0.0, 1.0);

    return (double) normalDistribution (mMersenneTwisterRNG);
}

/*
taggedMIDIBuffer[index].samplePosition = mMIDISampleOffsetBuffer[currentMidiMessage.getNoteNumber()]
                                                                   + midiMessageSamplePosition
                                                                   + mMaximumDelayTimeInSamples;
if (currentMidiMessage.isNoteOn())
{
    mMIDISampleOffsetBuffer[currentMidiMessage.getNoteNumber()] = generateNormalRandomNumber() * mTimingStandardDeviationInSamples;
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
*/