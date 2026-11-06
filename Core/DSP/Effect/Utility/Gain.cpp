#include <Core/DSP/Effect/Utility/Gain.h>

#include <Core/Utils/AudioMath.h>

namespace Core {

namespace {

constexpr ParameterInfo PARAMETERS[] = {
    ParameterInfo{ "gainDb", "Gain", "dB", -60.0f, 24.0f, 0.0f },
};

} // namespace

void Gain::prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    _smoothed.prepare( sampleRate );
    _smoothed.snapTo( _gain.load() );
}

void Gain::reset() {
    _smoothed.snapTo( _gain.load() );
}

void Gain::process( float* left, float* right, uint32_t frames ) {
    _smoothed.setTarget( _gain.load() );

    if ( right == nullptr ) {
        for ( uint32_t i = 0; i < frames; i++ ) {
            left[ i ] *= _smoothed.next();
        }
        return;
    }

    for ( uint32_t i = 0; i < frames; i++ ) {
        const float factor = _smoothed.next();
        left[ i ] *= factor;
        right[ i ] *= factor;
    }
}

void Gain::setParameter( std::string_view name, float value ) {
    if ( name == "gainDb" ) {
        _gain.store( AudioMath::decibelToLinear( AudioMath::clamp( value, -60.0f, 24.0f ) ) );
    }
}

float Gain::parameter( std::string_view name ) const {
    if ( name == "gainDb" ) {
        return AudioMath::linearToDecibel( _gain.load() );
    }
    return 0.0f;
}

std::span<const ParameterInfo> Gain::parameters() const {
    return PARAMETERS;
}

} // namespace Core
