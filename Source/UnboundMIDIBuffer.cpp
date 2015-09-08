#include "UnboundMIDIBuffer.h"

UnboundMIDIBuffer::UnboundMIDIBuffer()
{}

UnboundMIDIBuffer::~UnboundMIDIBuffer()
{}

void UnboundMIDIBuffer::addMessage (MidiMessage inputMIDIMessage, int inputSamplePosition)
{
    TaggedMIDIMessage temporaryTaggedMIDIMessage (inputMIDIMessage, inputSamplePosition);
    mInternalMIDIBuffer.push_back (temporaryTaggedMIDIMessage);
}

void UnboundMIDIBuffer::eraseMessage (int messageNumber)
{
    mInternalMIDIBuffer.erase (mInternalMIDIBuffer.begin() + messageNumber);
}

void UnboundMIDIBuffer::clear()
{
    mInternalMIDIBuffer.clear();
}

void UnboundMIDIBuffer::fixNoteOverlaps()
{
    int noteOnsInARow = 0;

    for (int index = 0; index < mInternalMIDIBuffer.size(); ++index)
    {

    }
}

TaggedMIDIMessage& UnboundMIDIBuffer::operator[] (int index) 
{ 
    return mInternalMIDIBuffer[index]; 
}