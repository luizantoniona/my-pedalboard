#include <Core/DSP/Effect/Distortion/Overdrive.h>

#include <cmath>
#include <initializer_list>

#include <Core/Utils/AudioMath.h>

namespace Core {

namespace {

constexpr float DRIVE_MIN = 1.0f;
constexpr float DRIVE_MAX = 20.0f;

constexpr float TONE_MIN_HZ = 700.0f;
constexpr float TONE_DECADES = 1.0f;

constexpr float INPUT_HIGHPASS_HZ = 100.0f;
constexpr float DC_BLOCK_HZ = 20.0f;

constexpr float HUMP_HZ = 720.0f;
constexpr float HUMP_Q = 0.8f;
constexpr float HUMP_GAIN_DB = 6.0f;

constexpr float DC_BLOCKER_HEADROOM = 0.75f;

constexpr ParameterInfo PARAMETERS[] = {
    ParameterInfo{ "drive", "Drive", "x", DRIVE_MIN, DRIVE_MAX, 4.0f },
    ParameterInfo{ "tone", "Tone", "", 0.0f, 1.0f, 0.5f },
    ParameterInfo{ "level", "Level", "dB", -24.0f, 6.0f, -6.0f },
    ParameterInfo{ "mix", "Mix", "", 0.0f, 1.0f, 1.0f },
};

} // namespace

void Overdrive::process( float* left, float* right, uint32_t frames ) {
    _driveSmoothed.setTarget( _drive.load() );
    _levelSmoothed.setTarget( _levelLinear.load() );
    _mixSmoothed.setTarget( _mix.load() );

    const float toneCoefficient = _toneCoefficient.load();

    for ( uint32_t i = 0; i < frames; i++ ) {
        const float drive = _driveSmoothed.next();
        const float level = _levelSmoothed.next();
        const float mix = _mixSmoothed.next();

        left[ i ] = shaped( _left, left[ i ], drive, level, mix, toneCoefficient );
        if ( right != nullptr ) {
            right[ i ] = shaped( _right, right[ i ], drive, level, mix, toneCoefficient );
        }
    }
}

void Overdrive::prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    _sampleRate.store( sampleRate );
    refreshTone();

    _inputHighpassCoefficient = AudioMath::onePoleCoefficient( INPUT_HIGHPASS_HZ, sampleRate );
    _dcCoefficient = AudioMath::onePoleCoefficient( DC_BLOCK_HZ, sampleRate );

    _left.hump.setPeaking( HUMP_HZ, HUMP_Q, HUMP_GAIN_DB, sampleRate );
    _right.hump.copyCoefficients( _left.hump );

    _driveSmoothed.prepare( sampleRate );
    _levelSmoothed.prepare( sampleRate );
    _mixSmoothed.prepare( sampleRate );

    reset();
}

void Overdrive::reset() {
    _driveSmoothed.snapTo( _drive.load() );
    _levelSmoothed.snapTo( _levelLinear.load() );
    _mixSmoothed.snapTo( _mix.load() );

    for ( Channel* channel : { &_left, &_right } ) {
        channel->hump.reset();
        channel->inputHighpassState = 0.0f;
        channel->toneState = 0.0f;
        channel->dcState = 0.0f;
    }
}

void Overdrive::setParameter( std::string_view name, float value ) {
    if ( name == "drive" ) {
        _drive.store( AudioMath::clamp( value, DRIVE_MIN, DRIVE_MAX ) );
    } else if ( name == "level" ) {
        _levelLinear.store( AudioMath::decibelToLinear( AudioMath::clamp( value, -24.0f, 6.0f ) ) );
    } else if ( name == "mix" ) {
        _mix.store( AudioMath::clamp( value, 0.0f, 1.0f ) );
    } else if ( name == "tone" ) {
        _tone.store( AudioMath::clamp( value, 0.0f, 1.0f ) );
        refreshTone();
    }
}

float Overdrive::parameter( std::string_view name ) const {
    if ( name == "drive" ) {
        return _drive.load();
    }
    if ( name == "level" ) {
        return AudioMath::linearToDecibel( _levelLinear.load() );
    }
    if ( name == "mix" ) {
        return _mix.load();
    }
    if ( name == "tone" ) {
        return _tone.load();
    }
    return 0.0f;
}

std::span<const ParameterInfo> Overdrive::parameters() const {
    return PARAMETERS;
}

float Overdrive::asymmetricClip( float sample ) const {
    const float biased = std::tanh( sample + ASYMMETRY_BIAS );
    return ( biased - _tanhOfBias ) / ( 1.0f + _tanhOfBias );
}

float Overdrive::shaped( Channel& channel, float sample, float drive, float level, float mix, float toneCoefficient ) const {
    channel.inputHighpassState =
        AudioMath::flushDenormal( channel.inputHighpassState + _inputHighpassCoefficient * ( sample - channel.inputHighpassState ) );
    const float tightened = sample - channel.inputHighpassState;

    const float driven = asymmetricClip( channel.hump.process( tightened ) * drive );

    channel.dcState = AudioMath::flushDenormal( channel.dcState + _dcCoefficient * ( driven - channel.dcState ) );
    const float centred = ( driven - channel.dcState ) * DC_BLOCKER_HEADROOM;

    channel.toneState = AudioMath::flushDenormal( channel.toneState + toneCoefficient * ( centred - channel.toneState ) );
    return AudioMath::lerp( sample, channel.toneState * level, mix );
}

void Overdrive::refreshTone() {
    const float cutoff = TONE_MIN_HZ * std::pow( 10.0f, TONE_DECADES * _tone.load() );
    _toneCoefficient.store( AudioMath::onePoleCoefficient( cutoff, _sampleRate.load() ) );
}

} // namespace Core
