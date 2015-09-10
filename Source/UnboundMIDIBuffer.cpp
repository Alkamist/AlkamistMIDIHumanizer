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

void UnboundMIDIBuffer::fixNoteOverlaps()
{
    for (int iii = 0; iii < mInternalNoteBuffer.size(); ++iii)
    {
        CompleteMIDINote& currentNote = mInternalNoteBuffer[iii];
        if (! currentNote.noteOff.isPlaceholder)
        {
            for (int jjj = 0; jjj < mInternalNoteBuffer.size(); ++jjj)
            {
                if (iii != jjj)
                {
                    const CompleteMIDINote& checkingNote = mInternalNoteBuffer[jjj];
                    const int& currentNoteNumber = currentNote.noteOn.message.getNoteNumber();
                    const int& checkingNoteNumber = checkingNote.noteOn.message.getNoteNumber();

                    if (currentNoteNumber == checkingNoteNumber)
                    {
                        if (currentNote.noteOn.samplePosition < checkingNote.noteOn.samplePosition)
                        {     
                            if (currentNote.noteOff.samplePosition > checkingNote.noteOn.samplePosition)
                            {
                                currentNote.noteOff.samplePosition = checkingNote.noteOn.samplePosition - 1;
                            }              
                        }
                    }
                }
            }
        }
    }
}

CompleteMIDINote& UnboundMIDIBuffer::operator[] (int index) 
{ 
    return mInternalNoteBuffer[index]; 
}