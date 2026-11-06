#pragma once

#include <cstdint>

#include <Core/Utils/AudioMath.h>

namespace Core {

class Biquad {

public:
    void setPeaking( float frequencyHz, float q, float gainDb, uint32_t sampleRate );
    void setLowShelf( float frequencyHz, float gainDb, uint32_t sampleRate );
    void setHighShelf( float frequencyHz, float gainDb, uint32_t sampleRate );

    void copyCoefficients( const Biquad& other );

    void reset() {
        _z1 = 0.0f;
        _z2 = 0.0f;
    }

    float process( float sample ) {
        const float output = _b0 * sample + _z1;
        _z1 = AudioMath::flushDenormal( _b1 * sample - _a1 * output + _z2 );
        _z2 = AudioMath::flushDenormal( _b2 * sample - _a2 * output );
        return output;
    }

private:
    void normalizeAndStore( float b0, float b1, float b2, float a0, float a1, float a2 );

    float _b0{ 1.0f };
    float _b1{ 0.0f };
    float _b2{ 0.0f };
    float _a1{ 0.0f };
    float _a2{ 0.0f };
    float _z1{ 0.0f };
    float _z2{ 0.0f };
};

} // namespace Core
