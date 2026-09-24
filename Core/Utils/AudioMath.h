#pragma once

#include <cmath>
#include <cstdint>

namespace Core {

class AudioMath {

public:
    static constexpr float SILENCE_DB = -144.0f;

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
};

} // namespace Core
