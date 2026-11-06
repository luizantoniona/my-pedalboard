#include <Tests/Support/SignalGenerator.h>

#include <cmath>

namespace TestSupport {

namespace {

constexpr float TWO_PI = 6.283185307179586f;
constexpr int HARMONIC_COUNT = 6;

} // namespace

std::vector<float> SignalGenerator::silence( uint32_t count ) {
    return std::vector<float>( count, 0.0f );
}

std::vector<float> SignalGenerator::constant( float value, uint32_t count ) {
    return std::vector<float>( count, value );
}

std::vector<float> SignalGenerator::ramp( uint32_t count ) {
    std::vector<float> samples( count );
    for ( uint32_t i = 0; i < count; i++ ) {
        samples[ i ] = static_cast<float>( i ) / static_cast<float>( count ) - 0.5f;
    }
    return samples;
}

std::vector<float> SignalGenerator::impulse( uint32_t count ) {
    std::vector<float> samples( count, 0.0f );
    if ( count > 0 ) {
        samples[ 0 ] = 1.0f;
    }
    return samples;
}

std::vector<float> SignalGenerator::sine( float frequency, uint32_t sampleRate, uint32_t count, float amplitude ) {
    std::vector<float> samples( count );
    const float step = TWO_PI * frequency / static_cast<float>( sampleRate );
    for ( uint32_t i = 0; i < count; i++ ) {
        samples[ i ] = amplitude * std::sin( step * static_cast<float>( i ) );
    }
    return samples;
}

std::vector<float> SignalGenerator::pluckedString( float frequency, uint32_t sampleRate, uint32_t count, float amplitude ) {
    std::vector<float> samples( count, 0.0f );

    for ( int harmonic = 1; harmonic <= HARMONIC_COUNT; harmonic++ ) {
        const float partial = frequency * static_cast<float>( harmonic );
        if ( partial >= static_cast<float>( sampleRate ) * 0.45f ) {
            break;
        }

        const float step = TWO_PI * partial / static_cast<float>( sampleRate );
        const float weight = amplitude / static_cast<float>( harmonic );
        const float decay = 2.0f * static_cast<float>( harmonic ) / static_cast<float>( sampleRate );

        for ( uint32_t i = 0; i < count; i++ ) {
            const float envelope = std::exp( -decay * static_cast<float>( i ) );
            samples[ i ] += weight * envelope * std::sin( step * static_cast<float>( i ) );
        }
    }

    return samples;
}

std::vector<float> SignalGenerator::whiteNoise( uint32_t count, uint32_t seed, float amplitude ) {
    std::vector<float> samples( count );
    uint32_t state = seed != 0 ? seed : 1;

    for ( uint32_t i = 0; i < count; i++ ) {
        state = state * 1664525u + 1013904223u;
        const float normalized = static_cast<float>( state >> 8 ) / static_cast<float>( 1u << 24 );
        samples[ i ] = amplitude * ( normalized * 2.0f - 1.0f );
    }

    return samples;
}

void SignalGenerator::addDcOffset( std::vector<float>& samples, float offset ) {
    for ( float& sample : samples ) {
        sample += offset;
    }
}

float SignalGenerator::peak( const std::vector<float>& samples ) {
    float highest = 0.0f;
    for ( float sample : samples ) {
        const float magnitude = std::fabs( sample );
        if ( magnitude > highest ) {
            highest = magnitude;
        }
    }
    return highest;
}

float SignalGenerator::rms( const std::vector<float>& samples ) {
    if ( samples.empty() ) {
        return 0.0f;
    }

    float sum = 0.0f;
    for ( float sample : samples ) {
        sum += sample * sample;
    }
    return std::sqrt( sum / static_cast<float>( samples.size() ) );
}

} // namespace TestSupport
