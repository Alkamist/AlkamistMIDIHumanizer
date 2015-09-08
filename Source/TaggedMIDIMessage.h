#ifndef TAGGEDMIDIMESSAGE_H_INCLUDED
#define TAGGEDMIDIMESSAGE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

struct TaggedMIDIMessage
{
    TaggedMIDIMessage (MidiMessage inputMIDIMessage, int inputSamplePosition);
              
    MidiMessage message;
    int samplePosition;
};

#endif  // TAGGEDMIDIMESSAGE_H_INCLUDED
