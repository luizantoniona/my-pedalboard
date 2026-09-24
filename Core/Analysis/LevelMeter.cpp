#include <Core/Analysis/LevelMeter.h>

#include <cmath>

namespace Core {

void LevelMeter::prepare( uint32_t sampleRate ) {
    _decayPerBlock = std::exp( -1.0f / ( 0.150f * static_cast<float>( sampleRate ) / 64.0f ) );
    reset();
}

void LevelMeter::process( const float* samples, uint32_t frames ) {
    float blockPeak = 0.0f;
    for ( uint32_t i = 0; i < frames; i++ ) {
        const float magnitude = std::fabs( samples[ i ] );
        if ( magnitude > blockPeak ) {
            blockPeak = magnitude;
        }
    }

    if ( blockPeak >= 0.999f ) {
        _clipping.store( true );
    }

    const float decayed = _peak.load() * _decayPerBlock;
    _peak.store( blockPeak > decayed ? blockPeak : decayed );
}

void LevelMeter::reset() {
    _peak.store( 0.0f );
    _clipping.store( false );
}

} // namespace Core
