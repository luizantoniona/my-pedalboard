#include <Core/DSP/Effect/Reverb/Reverb.h>

#include <algorithm>

#include <Core/Utils/AudioMath.h>

namespace Core {

namespace {

constexpr uint32_t REFERENCE_SAMPLE_RATE = 44100;

constexpr uint32_t COMB_TUNINGS[] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 };
constexpr uint32_t ALLPASS_TUNINGS[] = { 556, 441, 341, 225 };
constexpr uint32_t RIGHT_CHANNEL_SPREAD = 23;

constexpr float INPUT_GAIN = 0.015f;
constexpr float WET_GAIN = 1.5f;
constexpr float ALLPASS_FEEDBACK = 0.5f;

constexpr float FEEDBACK_MIN = 0.7f;
constexpr float FEEDBACK_RANGE = 0.26f;
constexpr float DAMPING_MAX = 0.4f;

constexpr ParameterInfo PARAMETERS[] = {
    ParameterInfo{ "size", "Size", "", 0.0f, 1.0f, 0.5f },
    ParameterInfo{ "damping", "Damping", "", 0.0f, 1.0f, 0.5f },
    ParameterInfo{ "mix", "Mix", "", 0.0f, 1.0f, 0.25f },
};

uint32_t scaledLength( uint32_t referenceLength, uint32_t sampleRate ) {
    const uint64_t scaled = static_cast<uint64_t>( referenceLength ) * sampleRate / REFERENCE_SAMPLE_RATE;
    return std::max<uint32_t>( 1, static_cast<uint32_t>( scaled ) );
}

} // namespace

void Reverb::process( float* left, float* right, uint32_t frames ) {
    if ( _combs[ 0 ][ 0 ].buffer.empty() ) {
        return;
    }

    _mixSmoothed.setTarget( _mix.load() );

    const float feedback = FEEDBACK_MIN + FEEDBACK_RANGE * _size.load();
    const float damping = DAMPING_MAX * _damping.load();

    for ( uint32_t i = 0; i < frames; i++ ) {
        const float mix = _mixSmoothed.next();

        const float dryLeft = left[ i ];
        const float dryRight = right != nullptr ? right[ i ] : dryLeft;
        const float input = ( dryLeft + dryRight ) * INPUT_GAIN;

        const float wetLeft = tank( 0, input, feedback, damping ) * WET_GAIN;
        const float wetRight = tank( 1, input, feedback, damping ) * WET_GAIN;

        left[ i ] = AudioMath::lerp( dryLeft, wetLeft, mix );
        if ( right != nullptr ) {
            right[ i ] = AudioMath::lerp( dryRight, wetRight, mix );
        }
    }
}

void Reverb::prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    for ( int channel = 0; channel < CHANNEL_COUNT; channel++ ) {
        const uint32_t spread = channel == 0 ? 0 : RIGHT_CHANNEL_SPREAD;

        for ( int comb = 0; comb < COMB_COUNT; comb++ ) {
            _combs[ channel ][ comb ].buffer.assign( scaledLength( COMB_TUNINGS[ comb ] + spread, sampleRate ), 0.0f );
        }
        for ( int allpass = 0; allpass < ALLPASS_COUNT; allpass++ ) {
            _allpasses[ channel ][ allpass ].buffer.assign( scaledLength( ALLPASS_TUNINGS[ allpass ] + spread, sampleRate ), 0.0f );
        }
    }

    _mixSmoothed.prepare( sampleRate );
    reset();
}

void Reverb::reset() {
    for ( int channel = 0; channel < CHANNEL_COUNT; channel++ ) {
        for ( DelayLine& comb : _combs[ channel ] ) {
            std::fill( comb.buffer.begin(), comb.buffer.end(), 0.0f );
            comb.filterState = 0.0f;
            comb.index = 0;
        }
        for ( DelayLine& allpass : _allpasses[ channel ] ) {
            std::fill( allpass.buffer.begin(), allpass.buffer.end(), 0.0f );
            allpass.index = 0;
        }
    }

    _mixSmoothed.snapTo( _mix.load() );
}

void Reverb::setParameter( std::string_view name, float value ) {
    if ( name == "size" ) {
        _size.store( AudioMath::clamp( value, 0.0f, 1.0f ) );
    } else if ( name == "damping" ) {
        _damping.store( AudioMath::clamp( value, 0.0f, 1.0f ) );
    } else if ( name == "mix" ) {
        _mix.store( AudioMath::clamp( value, 0.0f, 1.0f ) );
    }
}

float Reverb::parameter( std::string_view name ) const {
    if ( name == "size" ) {
        return _size.load();
    }
    if ( name == "damping" ) {
        return _damping.load();
    }
    if ( name == "mix" ) {
        return _mix.load();
    }
    return 0.0f;
}

std::span<const ParameterInfo> Reverb::parameters() const {
    return PARAMETERS;
}

float Reverb::combed( DelayLine& line, float input, float feedback, float damping ) const {
    const float output = line.buffer[ line.index ];
    line.filterState = AudioMath::flushDenormal( output * ( 1.0f - damping ) + line.filterState * damping );
    line.buffer[ line.index ] = input + line.filterState * feedback;
    line.index = ( line.index + 1 ) % static_cast<uint32_t>( line.buffer.size() );
    return output;
}

float Reverb::allpassed( DelayLine& line, float input ) const {
    const float buffered = line.buffer[ line.index ];
    line.buffer[ line.index ] = input + buffered * ALLPASS_FEEDBACK;
    line.index = ( line.index + 1 ) % static_cast<uint32_t>( line.buffer.size() );
    return buffered - input;
}

float Reverb::tank( int channel, float input, float feedback, float damping ) {
    float summed = 0.0f;
    for ( DelayLine& comb : _combs[ channel ] ) {
        summed += combed( comb, input, feedback, damping );
    }
    for ( DelayLine& allpass : _allpasses[ channel ] ) {
        summed = allpassed( allpass, summed );
    }
    return summed;
}

} // namespace Core
