#pragma once

namespace Core {

class NoteInfo {

public:
    int midiNote{ 0 };
    float cents{ 0.0f };
};

class NoteHelper {

public:
    static NoteInfo fromFrequency( float frequency );
    static const char* name( int midiNote );
    static int octave( int midiNote );
    static float frequency( int midiNote );
};

} // namespace Core
