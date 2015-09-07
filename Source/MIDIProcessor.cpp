#include "MIDIProcessor.h"

MIDIProcessor::MIDIProcessor()
    : mOutputMIDIBuffer(),
      mEmptyMIDIBuffer(),
      mCurrentMIDIMessage(),
      mCurrentMIDIMessageSamplePosition (0),
      mCurrentSampleIndex (0)
{
    mInputMIDIBuffer = &mEmptyMIDIBuffer;
}

MIDIProcessor::~MIDIProcessor()
{}

void MIDIProcessor::processPerBlock()
{
    mCurrentSampleIndex = 0;

    MidiBuffer::Iterator MIDIMessagesIterator (*mInputMIDIBuffer);

    if (! mInputMIDIBuffer->isEmpty())
    {
        MIDIMessagesIterator.getNextEvent (mCurrentMIDIMessage, mCurrentMIDIMessageSamplePosition);
    }
}

void MIDIProcessor::processPerSample()
{
    /*if (! mInputMIDIBuffer->isEmpty())
    {
        bool midiBufferIsNotEmpty = true;
        bool alreadyHadNoteOn = false;

        while (mCurrentSampleIndex == mCurrentMIDIMessageSamplePosition
               && midiBufferIsNotEmpty)
        {
            if (mCurrentMIDIMessage.isNoteOn()
                && ! alreadyHadNoteOn)
            {
                alreadyHadNoteOn = true;
            }

            midiBufferIsNotEmpty = MIDIMessagesIterator.getNextEvent (mCurrentMIDIMessage, mCurrentMIDIMessageSamplePosition);
        }
    }

    ++mCurrentSampleIndex;*/
}