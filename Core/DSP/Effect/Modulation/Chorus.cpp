#include <Core/DSP/Effect/Modulation/Chorus.h>

#include <algorithm>
#include <cmath>

#include <Core/Utils/AudioMath.h>

namespace Core {

namespace {

constexpr float RATE_MIN_HZ = 0.1f;
constexpr float RATE_MAX_HZ = 10.0f;

constexpr float CENTRE_DELAY_MS = 9.0f;
constexpr float SWING_DELAY_MS = 6.0f;
constexpr float LINE_LENGTH_MS = 20.0f;

constexpr float RIGHT_CHANNEL_PHASE_OFFSET = 0.25f;
constexpr float TWO_PI = 6.2831853f;

constexpr uint32_t INTERPOLATION_GUARD_SAMPLES = 2;

constexpr ParameterInfo PARAMETERS[] = {
    ParameterInfo{ "rate", "Rate", "Hz", RATE_MIN_HZ, RATE_MAX_HZ, 0.8f },
    ParameterInfo{ "depth", "Depth", "", 0.0f, 1.0f, 0.5f },
    ParameterInfo{ "mix", "Mix", "", 0.0f, 1.0f, 0.5f },
};

float wrappedPhase( float phase ) {
    return phase - std::floor( phase );
}

} // namespace

void Chorus::process( float* left, float* right, uint32_t frames ) {
    const uint32_t lineLength = static_cast<uint32_t>( _lineLeft.size() );
    if ( lineLength == 0 ) {
        return;
    }

    _mixSmoothed.setTarget( _mix.load() );

    const float depth = _depth.load();
    const float phaseIncrement = _rateHz.load() / static_cast<float>( _sampleRate.load() );

    for ( uint32_t i = 0; i < frames; i++ ) {
        const float mix = _mixSmoothed.next();

        const float dryLeft = left[ i ];
        _lineLeft[ _writeIndex ] = dryLeft;
        const float wetLeft = modulated( _lineLeft, _writeIndex, delaySamplesAt( _lfoPhase, depth ) );
        left[ i ] = AudioMath::lerp( dryLeft, wetLeft, mix );

        if ( right != nullptr ) {
            const float dryRight = right[ i ];
            _lineRight[ _writeIndex ] = dryRight;
            const float rightPhase = wrappedPhase( _lfoPhase + RIGHT_CHANNEL_PHASE_OFFSET );
            const float wetRight = modulated( _lineRight, _writeIndex, delaySamplesAt( rightPhase, depth ) );
            right[ i ] = AudioMath::lerp( dryRight, wetRight, mix );
        }

        _lfoPhase = wrappedPhase( _lfoPhase + phaseIncrement );
        _writeIndex = ( _writeIndex + 1 ) % lineLength;
    }
}

void Chorus::prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    _sampleRate.store( sampleRate );

    const uint32_t lineLength =
        static_cast<uint32_t>( LINE_LENGTH_MS * static_cast<float>( sampleRate ) / 1000.0f ) + INTERPOLATION_GUARD_SAMPLES;
    _lineLeft.assign( lineLength, 0.0f );
    _lineRight.assign( lineLength, 0.0f );

    _mixSmoothed.prepare( sampleRate );
    reset();
}

void Chorus::reset() {
    std::fill( _lineLeft.begin(), _lineLeft.end(), 0.0f );
    std::fill( _lineRight.begin(), _lineRight.end(), 0.0f );
    _lfoPhase = 0.0f;
    _writeIndex = 0;

    _mixSmoothed.snapTo( _mix.load() );
}

void Chorus::setParameter( std::string_view name, float value ) {
    if ( name == "rate" ) {
        _rateHz.store( AudioMath::clamp( value, RATE_MIN_HZ, RATE_MAX_HZ ) );
    } else if ( name == "depth" ) {
        _depth.store( AudioMath::clamp( value, 0.0f, 1.0f ) );
    } else if ( name == "mix" ) {
        _mix.store( AudioMath::clamp( value, 0.0f, 1.0f ) );
    }
}

float Chorus::parameter( std::string_view name ) const {
    if ( name == "rate" ) {
        return _rateHz.load();
    }
    if ( name == "depth" ) {
        return _depth.load();
    }
    if ( name == "mix" ) {
        return _mix.load();
    }
    return 0.0f;
}

std::span<const ParameterInfo> Chorus::parameters() const {
    return PARAMETERS;
}

float Chorus::modulated( const std::vector<float>& line, uint32_t writeIndex, float delaySamples ) const {
    const uint32_t lineLength = static_cast<uint32_t>( line.size() );

    const uint32_t wholeDelay = static_cast<uint32_t>( delaySamples );
    const float fraction = delaySamples - static_cast<float>( wholeDelay );

    const uint32_t newer = ( writeIndex + lineLength - wholeDelay ) % lineLength;
    const uint32_t older = ( newer + lineLength - 1 ) % lineLength;
    return AudioMath::lerp( line[ newer ], line[ older ], fraction );
}

float Chorus::delaySamplesAt( float lfoPhase, float depth ) const {
    const float samplesPerMillisecond = static_cast<float>( _sampleRate.load() ) / 1000.0f;
    const float delayMilliseconds = CENTRE_DELAY_MS + SWING_DELAY_MS * depth * std::sin( TWO_PI * lfoPhase );
    return delayMilliseconds * samplesPerMillisecond;
}

} // namespace Core
