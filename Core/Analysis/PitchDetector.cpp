#include <Core/Analysis/PitchDetector.h>

#include <cmath>

namespace Core {

void PitchDetector::prepare( uint32_t sampleRate ) {
    _sampleRate = sampleRate;
    _decimation = sampleRate / 12000;
    if ( _decimation < 1 ) {
        _decimation = 1;
    }
    _workingRate = sampleRate / _decimation;

    _tauMax = 256;
    _tauMin = _workingRate / 1200;
    if ( _tauMin < 2 ) {
        _tauMin = 2;
    }

    _inputSize = _tauMax * 2 * _decimation;

    _working.assign( _tauMax * 2, 0.0f );
    _difference.assign( _tauMax, 0.0f );
    _cumulative.assign( _tauMax, 0.0f );
}

PitchDetector::Result PitchDetector::analyze( const float* samples, uint32_t count ) {
    Result result;
    if ( count < _inputSize ) {
        return result;
    }

    decimate( samples );

    float sumSquares = 0.0f;
    for ( float value : _working ) {
        sumSquares += value * value;
    }
    const float rms = std::sqrt( sumSquares / static_cast<float>( _working.size() ) );
    if ( rms < SILENCE_RMS ) {
        return result;
    }

    computeDifference();
    computeCumulativeMean();

    const uint32_t tau = absoluteThreshold();
    if ( tau == 0 ) {
        return result;
    }

    const float refined = parabolicInterpolation( tau );
    if ( refined <= 0.0f ) {
        return result;
    }

    result.frequency = static_cast<float>( _workingRate ) / refined;
    result.clarity = 1.0f - _cumulative[ tau ];
    result.voiced = true;
    return result;
}

void PitchDetector::decimate( const float* samples ) {
    const uint32_t windowSamples = static_cast<uint32_t>( _working.size() );

    float mean = 0.0f;
    for ( uint32_t i = 0; i < windowSamples; i++ ) {
        float accumulated = 0.0f;
        for ( uint32_t j = 0; j < _decimation; j++ ) {
            accumulated += samples[ i * _decimation + j ];
        }
        const float value = accumulated / static_cast<float>( _decimation );
        _working[ i ] = value;
        mean += value;
    }

    mean /= static_cast<float>( windowSamples );
    for ( uint32_t i = 0; i < windowSamples; i++ ) {
        _working[ i ] -= mean;
    }
}

void PitchDetector::computeDifference() {
    _difference[ 0 ] = 0.0f;
    for ( uint32_t tau = 1; tau < _tauMax; tau++ ) {
        float sum = 0.0f;
        for ( uint32_t j = 0; j < _tauMax; j++ ) {
            const float delta = _working[ j ] - _working[ j + tau ];
            sum += delta * delta;
        }
        _difference[ tau ] = sum;
    }
}

void PitchDetector::computeCumulativeMean() {
    _cumulative[ 0 ] = 1.0f;
    float runningSum = 0.0f;
    for ( uint32_t tau = 1; tau < _tauMax; tau++ ) {
        runningSum += _difference[ tau ];
        _cumulative[ tau ] = runningSum > 0.0f
                                 ? _difference[ tau ] * static_cast<float>( tau ) / runningSum
                                 : 1.0f;
    }
}

uint32_t PitchDetector::absoluteThreshold() const {
    for ( uint32_t tau = _tauMin; tau < _tauMax - 1; tau++ ) {
        if ( _cumulative[ tau ] >= THRESHOLD ) {
            continue;
        }
        uint32_t candidate = tau;
        while ( candidate + 1 < _tauMax && _cumulative[ candidate + 1 ] < _cumulative[ candidate ] ) {
            candidate++;
        }
        return candidate;
    }
    return 0;
}

float PitchDetector::parabolicInterpolation( uint32_t tau ) const {
    if ( tau == 0 || tau + 1 >= _tauMax ) {
        return static_cast<float>( tau );
    }

    const float previous = _cumulative[ tau - 1 ];
    const float centre = _cumulative[ tau ];
    const float next = _cumulative[ tau + 1 ];

    const float denominator = 2.0f * ( 2.0f * centre - next - previous );
    if ( std::fabs( denominator ) < 1.0e-9f ) {
        return static_cast<float>( tau );
    }

    return static_cast<float>( tau ) + ( next - previous ) / denominator;
}

} // namespace Core
