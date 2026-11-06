#include <Core/DSP/Effect/Delay/Delay.h>

#include <algorithm>
#include <cmath>

#include <Core/Utils/AudioMath.h>

namespace Core {

namespace {

constexpr float TIME_MIN_MS = 20.0f;
constexpr float TIME_MAX_MS = 1500.0f;

constexpr float FEEDBACK_MAX = 0.95f;

constexpr float TONE_MIN_HZ = 1000.0f;
constexpr float TONE_DECADES = 1.0f;

constexpr float TIME_RAMP_MS = 80.0f;

constexpr uint32_t INTERPOLATION_GUARD_SAMPLES = 2;

constexpr ParameterInfo PARAMETERS[] = {
    ParameterInfo{ "time", "Time", "ms", TIME_MIN_MS, TIME_MAX_MS, 380.0f },
    ParameterInfo{ "feedback", "Feedback", "", 0.0f, FEEDBACK_MAX, 0.35f },
    ParameterInfo{ "tone", "Tone", "", 0.0f, 1.0f, 0.6f },
    ParameterInfo{ "mix", "Mix", "", 0.0f, 1.0f, 0.3f },
};

} // namespace

void Delay::process( float* left, float* right, uint32_t frames ) {
    const uint32_t lineLength = static_cast<uint32_t>( _left.line.size() );
    if ( lineLength == 0 ) {
        return;
    }

    _timeSmoothed.setTarget( _timeMilliseconds.load() );
    _mixSmoothed.setTarget( _mix.load() );

    const float feedback = _feedback.load();
    const float toneCoefficient = _toneCoefficient.load();
    const float samplesPerMillisecond = static_cast<float>( _sampleRate.load() ) / 1000.0f;

    for ( uint32_t i = 0; i < frames; i++ ) {
        const float delaySamples = _timeSmoothed.next() * samplesPerMillisecond;
        const float mix = _mixSmoothed.next();

        const float dryLeft = left[ i ];
        const float wetLeft = echoed( _left, dryLeft, _writeIndex, delaySamples, feedback, toneCoefficient );
        left[ i ] = dryLeft + wetLeft * mix;

        if ( right != nullptr ) {
            const float dryRight = right[ i ];
            const float wetRight = echoed( _right, dryRight, _writeIndex, delaySamples, feedback, toneCoefficient );
            right[ i ] = dryRight + wetRight * mix;
        }

        _writeIndex = ( _writeIndex + 1 ) % lineLength;
    }
}

void Delay::prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    _sampleRate.store( sampleRate );

    const uint32_t lineLength =
        static_cast<uint32_t>( TIME_MAX_MS * 0.001f * static_cast<float>( sampleRate ) ) + INTERPOLATION_GUARD_SAMPLES;
    _left.line.assign( lineLength, 0.0f );
    _right.line.assign( lineLength, 0.0f );

    _timeSmoothed.prepare( sampleRate, TIME_RAMP_MS );
    _mixSmoothed.prepare( sampleRate );

    refreshTone();
    reset();
}

void Delay::reset() {
    std::fill( _left.line.begin(), _left.line.end(), 0.0f );
    std::fill( _right.line.begin(), _right.line.end(), 0.0f );
    _left.feedbackToneState = 0.0f;
    _right.feedbackToneState = 0.0f;
    _writeIndex = 0;

    _timeSmoothed.snapTo( _timeMilliseconds.load() );
    _mixSmoothed.snapTo( _mix.load() );
}

void Delay::setParameter( std::string_view name, float value ) {
    if ( name == "time" ) {
        _timeMilliseconds.store( AudioMath::clamp( value, TIME_MIN_MS, TIME_MAX_MS ) );
    } else if ( name == "feedback" ) {
        _feedback.store( AudioMath::clamp( value, 0.0f, FEEDBACK_MAX ) );
    } else if ( name == "tone" ) {
        _tone.store( AudioMath::clamp( value, 0.0f, 1.0f ) );
        refreshTone();
    } else if ( name == "mix" ) {
        _mix.store( AudioMath::clamp( value, 0.0f, 1.0f ) );
    }
}

float Delay::parameter( std::string_view name ) const {
    if ( name == "time" ) {
        return _timeMilliseconds.load();
    }
    if ( name == "feedback" ) {
        return _feedback.load();
    }
    if ( name == "tone" ) {
        return _tone.load();
    }
    if ( name == "mix" ) {
        return _mix.load();
    }
    return 0.0f;
}

std::span<const ParameterInfo> Delay::parameters() const {
    return PARAMETERS;
}

float Delay::echoed( Channel& channel, float sample, uint32_t writeIndex, float delaySamples, float feedback, float toneCoefficient ) const {
    const uint32_t lineLength = static_cast<uint32_t>( channel.line.size() );
    const float boundedDelay = AudioMath::clamp( delaySamples, 1.0f, static_cast<float>( lineLength - INTERPOLATION_GUARD_SAMPLES ) );

    const uint32_t wholeDelay = static_cast<uint32_t>( boundedDelay );
    const float fraction = boundedDelay - static_cast<float>( wholeDelay );

    const uint32_t newer = ( writeIndex + lineLength - wholeDelay ) % lineLength;
    const uint32_t older = ( newer + lineLength - 1 ) % lineLength;
    const float delayed = AudioMath::lerp( channel.line[ newer ], channel.line[ older ], fraction );

    channel.feedbackToneState =
        AudioMath::flushDenormal( channel.feedbackToneState + toneCoefficient * ( delayed - channel.feedbackToneState ) );
    channel.line[ writeIndex ] = sample + channel.feedbackToneState * feedback;

    return delayed;
}

void Delay::refreshTone() {
    const float cutoff = TONE_MIN_HZ * std::pow( 10.0f, TONE_DECADES * _tone.load() );
    _toneCoefficient.store( AudioMath::onePoleCoefficient( cutoff, _sampleRate.load() ) );
}

} // namespace Core
