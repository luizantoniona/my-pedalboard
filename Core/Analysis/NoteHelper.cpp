#include <Core/Analysis/NoteHelper.h>

#include <cmath>

namespace Core {

namespace {

const char* const NOTE_NAMES[ 12 ] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

constexpr float A4_FREQUENCY = 440.0f;
constexpr int A4_MIDI = 69;

} // namespace

NoteInfo NoteHelper::fromFrequency( float frequency ) {
    NoteInfo info;
    if ( frequency <= 0.0f ) {
        return info;
    }

    const float exact = A4_MIDI + 12.0f * std::log2( frequency / A4_FREQUENCY );
    info.midiNote = static_cast<int>( std::lround( exact ) );
    info.cents = ( exact - static_cast<float>( info.midiNote ) ) * 100.0f;
    return info;
}

const char* NoteHelper::name( int midiNote ) {
    if ( midiNote < 0 ) {
        return "-";
    }
    return NOTE_NAMES[ midiNote % 12 ];
}

int NoteHelper::octave( int midiNote ) {
    return midiNote / 12 - 1;
}

float NoteHelper::frequency( int midiNote ) {
    return A4_FREQUENCY * std::pow( 2.0f, static_cast<float>( midiNote - A4_MIDI ) / 12.0f );
}

} // namespace Core
