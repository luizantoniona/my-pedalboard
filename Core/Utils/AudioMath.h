#pragma once

#include <cmath>
#include <cstdint>

namespace Core {

class AudioMath {

public:
    static constexpr float SILENCE_DB = -144.0f;

    static constexpr float DENORMAL_FLOOR = 1.0e-20f;

    static float decibelToLinear( float decibels ) {
        return std::pow( 10.0f, decibels / 20.0f );
    }

    static float linearToDecibel( float linear ) {
        if ( linear <= 0.0f ) {
            return SILENCE_DB;
        }
        return 20.0f * std::log10( linear );
    }

    static float clamp( float value, float minimum, float maximum ) {
        return value < minimum ? minimum : ( value > maximum ? maximum : value );
    }

    static float hardClip( float sample ) {
        return clamp( sample, -1.0f, 1.0f );
    }

    static float lerp( float from, float to, float position ) {
        return from + position * ( to - from );
    }

    static float flushDenormal( float value ) {
        return std::fabs( value ) < DENORMAL_FLOOR ? 0.0f : value;
    }

    static float onePoleCoefficient( float cutoffHz, uint32_t sampleRate ) {
        if ( sampleRate == 0 ) {
            return 1.0f;
        }
        const float normalized = cutoffHz / static_cast<float>( sampleRate );
        return clamp( 1.0f - std::exp( -6.2831853f * normalized ), 0.0f, 1.0f );
    }

    static float rampCoefficient( float milliseconds, uint32_t sampleRate ) {
        const float samples = ( milliseconds * 0.001f ) * static_cast<float>( sampleRate );
        return samples > 1.0f ? std::exp( -1.0f / samples ) : 0.0f;
    }
};

} // namespace Core
