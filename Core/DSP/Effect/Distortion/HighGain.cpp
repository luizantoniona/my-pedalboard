#include <Core/DSP/Effect/Distortion/HighGain.h>

#include <cmath>
#include <cstring>
#include <initializer_list>

#include <Core/Utils/AudioMath.h>

namespace Core {

namespace {

constexpr float PRE_GAIN_MIN_DB = 6.0f;
constexpr float PRE_GAIN_RANGE_DB = 24.0f;
constexpr float SECOND_STAGE_GAIN = 8.0f;

constexpr float INPUT_HIGHPASS_HZ = 120.0f;
constexpr float INTERSTAGE_LOWPASS_HZ = 5000.0f;
constexpr float OUTPUT_LOWPASS_HZ = 9000.0f;

constexpr float LOW_SHELF_HZ = 100.0f;
constexpr float HIGH_SHELF_HZ = 10000.0f;
constexpr float MID_Q = 1.2f;

constexpr float TONE_LIMIT_DB = 15.0f;

constexpr int LOW_INDEX = 0;
constexpr int MID_INDEX = 1;
constexpr int MID_FREQUENCY_INDEX = 2;
constexpr int HIGH_INDEX = 3;

constexpr ParameterInfo PARAMETERS[] = {
    ParameterInfo{ "distortion", "Dist", "", 0.0f, 1.0f, 0.7f },
    ParameterInfo{ "low", "Low", "dB", -TONE_LIMIT_DB, TONE_LIMIT_DB, 0.0f },
    ParameterInfo{ "mid", "Mid", "dB", -TONE_LIMIT_DB, TONE_LIMIT_DB, 0.0f },
    ParameterInfo{ "midFrequency", "Mid Freq", "Hz", 200.0f, 5000.0f, 800.0f },
    ParameterInfo{ "high", "High", "dB", -TONE_LIMIT_DB, TONE_LIMIT_DB, 0.0f },
    ParameterInfo{ "level", "Level", "dB", -24.0f, 6.0f, -6.0f },
};

float preGainFor( float distortion ) {
    return AudioMath::decibelToLinear( PRE_GAIN_MIN_DB + PRE_GAIN_RANGE_DB * distortion );
}

float smoothClip( float sample ) {
    return sample / std::sqrt( 1.0f + sample * sample );
}

} // namespace

void HighGain::process( float* left, float* right, uint32_t frames ) {
    const float toneSnapshot[ TONE_VALUE_COUNT ] = {
        _lowDb.load(),
        _midDb.load(),
        _midFrequencyHz.load(),
        _highDb.load(),
    };

    if ( !_hasAppliedTone || std::memcmp( toneSnapshot, _appliedTone, sizeof( toneSnapshot ) ) != 0 ) {
        redesignTone( toneSnapshot );
    }

    _preGainSmoothed.setTarget( _preGainLinear.load() );
    _levelSmoothed.setTarget( _levelLinear.load() );

    for ( uint32_t i = 0; i < frames; i++ ) {
        const float preGain = _preGainSmoothed.next();
        const float level = _levelSmoothed.next();

        left[ i ] = shaped( _left, left[ i ], preGain, level );
        if ( right != nullptr ) {
            right[ i ] = shaped( _right, right[ i ], preGain, level );
        }
    }
}

void HighGain::prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    _sampleRate.store( sampleRate );
    _hasAppliedTone = false;

    _inputHighpassCoefficient = AudioMath::onePoleCoefficient( INPUT_HIGHPASS_HZ, sampleRate );
    _interstageCoefficient = AudioMath::onePoleCoefficient( INTERSTAGE_LOWPASS_HZ, sampleRate );
    _outputCoefficient = AudioMath::onePoleCoefficient( OUTPUT_LOWPASS_HZ, sampleRate );

    _preGainSmoothed.prepare( sampleRate );
    _levelSmoothed.prepare( sampleRate );

    reset();
}

void HighGain::reset() {
    _preGainSmoothed.snapTo( _preGainLinear.load() );
    _levelSmoothed.snapTo( _levelLinear.load() );

    for ( Channel* channel : { &_left, &_right } ) {
        channel->low.reset();
        channel->mid.reset();
        channel->high.reset();
        channel->inputHighpassState = 0.0f;
        channel->interstageState = 0.0f;
        channel->outputState = 0.0f;
    }
}

void HighGain::setParameter( std::string_view name, float value ) {
    if ( name == "distortion" ) {
        const float clamped = AudioMath::clamp( value, 0.0f, 1.0f );
        _distortion.store( clamped );
        _preGainLinear.store( preGainFor( clamped ) );
    } else if ( name == "low" ) {
        _lowDb.store( AudioMath::clamp( value, -TONE_LIMIT_DB, TONE_LIMIT_DB ) );
    } else if ( name == "mid" ) {
        _midDb.store( AudioMath::clamp( value, -TONE_LIMIT_DB, TONE_LIMIT_DB ) );
    } else if ( name == "midFrequency" ) {
        _midFrequencyHz.store( AudioMath::clamp( value, 200.0f, 5000.0f ) );
    } else if ( name == "high" ) {
        _highDb.store( AudioMath::clamp( value, -TONE_LIMIT_DB, TONE_LIMIT_DB ) );
    } else if ( name == "level" ) {
        _levelLinear.store( AudioMath::decibelToLinear( AudioMath::clamp( value, -24.0f, 6.0f ) ) );
    }
}

float HighGain::parameter( std::string_view name ) const {
    if ( name == "distortion" ) {
        return _distortion.load();
    }
    if ( name == "low" ) {
        return _lowDb.load();
    }
    if ( name == "mid" ) {
        return _midDb.load();
    }
    if ( name == "midFrequency" ) {
        return _midFrequencyHz.load();
    }
    if ( name == "high" ) {
        return _highDb.load();
    }
    if ( name == "level" ) {
        return AudioMath::linearToDecibel( _levelLinear.load() );
    }
    return 0.0f;
}

std::span<const ParameterInfo> HighGain::parameters() const {
    return PARAMETERS;
}

float HighGain::shaped( Channel& channel, float sample, float preGain, float level ) const {
    channel.inputHighpassState =
        AudioMath::flushDenormal( channel.inputHighpassState + _inputHighpassCoefficient * ( sample - channel.inputHighpassState ) );
    const float tightened = sample - channel.inputHighpassState;

    const float firstStage = std::tanh( tightened * preGain );

    channel.interstageState =
        AudioMath::flushDenormal( channel.interstageState + _interstageCoefficient * ( firstStage - channel.interstageState ) );
    const float secondStage = smoothClip( channel.interstageState * SECOND_STAGE_GAIN );

    const float toned = channel.high.process( channel.mid.process( channel.low.process( secondStage ) ) );

    channel.outputState = AudioMath::flushDenormal( channel.outputState + _outputCoefficient * ( toned - channel.outputState ) );
    return channel.outputState * level;
}

void HighGain::redesignTone( const float* toneSnapshot ) {
    const uint32_t sampleRate = _sampleRate.load();

    _left.low.setLowShelf( LOW_SHELF_HZ, toneSnapshot[ LOW_INDEX ], sampleRate );
    _left.mid.setPeaking( toneSnapshot[ MID_FREQUENCY_INDEX ], MID_Q, toneSnapshot[ MID_INDEX ], sampleRate );
    _left.high.setHighShelf( HIGH_SHELF_HZ, toneSnapshot[ HIGH_INDEX ], sampleRate );

    _right.low.copyCoefficients( _left.low );
    _right.mid.copyCoefficients( _left.mid );
    _right.high.copyCoefficients( _left.high );

    std::memcpy( _appliedTone, toneSnapshot, sizeof( _appliedTone ) );
    _hasAppliedTone = true;
}

} // namespace Core
