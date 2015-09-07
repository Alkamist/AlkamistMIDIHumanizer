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

void UnboundMIDIBuffer::clear()
{
    mInternalMIDIBuffer.clear();
}