#include "UnboundMIDIBuffer.h"

UnboundMIDIBuffer::UnboundMIDIBuffer()
{}

UnboundMIDIBuffer::~UnboundMIDIBuffer()
{}

void UnboundMIDIBuffer::addNoteOn (const TaggedMIDIMessage& inputMessage)
{
    TaggedMIDIMessage placeholderMessage (true);

    CompleteMIDINote noteStart (inputMessage, placeholderMessage);
    mInternalNoteBuffer.push_back (noteStart);
}

void UnboundMIDIBuffer::addNoteOff (const TaggedMIDIMessage& inputMessage)
{
    int inputNoteNumber = inputMessage.message.getNoteNumber();

    for (int index = 0; index < mInternalNoteBuffer.size(); ++index)
    {
        if (mInternalNoteBuffer[index].noteOff.isPlaceholder
        && (mInternalNoteBuffer[index].noteOn.message.getNoteNumber() == inputNoteNumber))
        {
            mInternalNoteBuffer[index].noteOff = inputMessage;
            break;
        }
    }
}

void UnboundMIDIBuffer::addCompleteMessage (const CompleteMIDINote& inputMessage)
{
    mInternalNoteBuffer.push_back (inputMessage);
}

void UnboundMIDIBuffer::removeCompleteMessage (int messageNumber)
{
    mInternalNoteBuffer.erase (mInternalNoteBuffer.begin() + messageNumber);
}

void UnboundMIDIBuffer::clear()
{
    mInternalNoteBuffer.clear();
}

/*void UnboundMIDIBuffer::fixNoteOverlaps()
{
    int noteOnsInARow = 0;

    for (int index = 0; index < mInternalNoteBuffer.size(); ++index)
    {

    }
}*/

CompleteMIDINote& UnboundMIDIBuffer::operator[] (int index) 
{ 
    return mInternalNoteBuffer[index]; 
}