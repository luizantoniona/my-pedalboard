#pragma once

#include <cmath>
#include <cstdint>

namespace Engine::AudioMath {

constexpr float kSilenceDb = -144.0f;

inline float dbToLinear( float db ) noexcept {
    return std::pow( 10.0f, db / 20.0f );
}

inline float linearToDb( float linear ) noexcept {
    if ( linear <= 0.0f )
        return kSilenceDb;
    return 20.0f * std::log10( linear );
}

// Peak level of an interleaved stereo buffer
inline float peakLevel( const float* buffer, uint32_t frames, uint32_t channels = 2 ) noexcept {
    float peak = 0.0f;
    for ( uint32_t i = 0; i < frames * channels; i++ ) {
        const float abs = std::abs( buffer[ i ] );
        if ( abs > peak )
            peak = abs;
    }
    return peak;
}

// Hard clamp to [-1, 1]
inline float hardClip( float sample ) noexcept {
    return sample < -1.0f ? -1.0f : ( sample > 1.0f ? 1.0f : sample );
}

// Linear interpolation
inline float lerp( float a, float b, float t ) noexcept {
    return a + t * ( b - a );
}

} // namespace Engine::AudioMath
