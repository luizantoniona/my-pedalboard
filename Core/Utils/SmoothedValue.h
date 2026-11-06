#pragma once

#include <cmath>
#include <cstdint>

namespace Core {

class SmoothedValue {

public:
    void prepare( uint32_t sampleRate, float rampMilliseconds = 20.0f ) {
        const float samples = ( rampMilliseconds * 0.001f ) * static_cast<float>( sampleRate );
        _coefficient = samples > 1.0f ? std::exp( -1.0f / samples ) : 0.0f;
    }

    void setTarget( float target ) {
        _target = target;
    }

    void snapTo( float value ) {
        _target = value;
        _current = value;
    }

    float next() {
        _current = _target + ( _current - _target ) * _coefficient;
        return _current;
    }

    float current() const {
        return _current;
    }

    bool isSettled() const {
        return std::fabs( _current - _target ) < 1.0e-6f;
    }

private:
    float _coefficient{ 0.0f };
    float _target{ 0.0f };
    float _current{ 0.0f };
};

} // namespace Core
