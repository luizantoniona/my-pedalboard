#include <Core/DSP/Effect/Dynamics/NoiseGate.h>

#include <cmath>

#include <Core/Utils/AudioMath.h>

namespace Core {

namespace {

constexpr float DETECTOR_RELEASE_MS = 15.0f;

constexpr ParameterInfo PARAMETERS[] = {
    ParameterInfo{ "threshold", "Threshold", "dB", -80.0f, 0.0f, -60.0f },
    ParameterInfo{ "attack", "Attack", "ms", 0.1f, 50.0f, 2.0f },
    ParameterInfo{ "release", "Release", "ms", 5.0f, 1000.0f, 120.0f },
    ParameterInfo{ "hold", "Hold", "ms", 0.0f, 500.0f, 40.0f },
};

} // namespace

void NoiseGate::process( float* left, float* right, uint32_t frames ) {
    const float threshold = _threshold.load();
    const float attackCoefficient = _attackCoefficient.load();
    const float releaseCoefficient = _releaseCoefficient.load();
    const float detectorCoefficient = _detectorCoefficient.load();
    const int holdSamples = _holdSamples.load();

    for ( uint32_t i = 0; i < frames; i++ ) {
        float magnitude = std::fabs( left[ i ] );
        if ( right != nullptr ) {
            magnitude = std::fmax( magnitude, std::fabs( right[ i ] ) );
        }

        _envelope = magnitude > _envelope
                        ? magnitude
                        : AudioMath::flushDenormal( _envelope * detectorCoefficient );

        if ( _envelope > threshold ) {
            _holdCounter = holdSamples;
        } else if ( _holdCounter > 0 ) {
            _holdCounter--;
        }

        const float target = ( _envelope > threshold || _holdCounter > 0 ) ? 1.0f : 0.0f;
        const float coefficient = target > _gateGain ? attackCoefficient : releaseCoefficient;
        _gateGain = AudioMath::flushDenormal( target + ( _gateGain - target ) * coefficient );

        left[ i ] *= _gateGain;
        if ( right != nullptr ) {
            right[ i ] *= _gateGain;
        }
    }
}

void NoiseGate::prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    _sampleRate.store( sampleRate );
    refreshCoefficients();
    reset();
}

void NoiseGate::reset() {
    _envelope = 0.0f;
    _gateGain = 0.0f;
    _holdCounter = 0;
}

void NoiseGate::setParameter( std::string_view name, float value ) {
    if ( name == "threshold" ) {
        _threshold.store( AudioMath::decibelToLinear( AudioMath::clamp( value, -80.0f, 0.0f ) ) );
        return;
    }
    if ( name == "attack" ) {
        _attackMs.store( AudioMath::clamp( value, 0.1f, 50.0f ) );
    } else if ( name == "release" ) {
        _releaseMs.store( AudioMath::clamp( value, 5.0f, 1000.0f ) );
    } else if ( name == "hold" ) {
        _holdMs.store( AudioMath::clamp( value, 0.0f, 500.0f ) );
    } else {
        return;
    }
    refreshCoefficients();
}

float NoiseGate::parameter( std::string_view name ) const {
    if ( name == "threshold" ) {
        return AudioMath::linearToDecibel( _threshold.load() );
    }
    if ( name == "attack" ) {
        return _attackMs.load();
    }
    if ( name == "release" ) {
        return _releaseMs.load();
    }
    if ( name == "hold" ) {
        return _holdMs.load();
    }
    return 0.0f;
}

std::span<const ParameterInfo> NoiseGate::parameters() const {
    return PARAMETERS;
}

void NoiseGate::refreshCoefficients() {
    const uint32_t sampleRate = _sampleRate.load();
    _attackCoefficient.store( AudioMath::rampCoefficient( _attackMs.load(), sampleRate ) );
    _releaseCoefficient.store( AudioMath::rampCoefficient( _releaseMs.load(), sampleRate ) );
    _detectorCoefficient.store( AudioMath::rampCoefficient( DETECTOR_RELEASE_MS, sampleRate ) );
    _holdSamples.store( static_cast<int>( ( _holdMs.load() * 0.001f ) * static_cast<float>( sampleRate ) ) );
}

} // namespace Core
