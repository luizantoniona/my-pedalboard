#pragma once

#include <cstdint>
#include <vector>

namespace TestSupport {

// Deterministic synthetic input for tests. No Qt and no hardware, so it is
// usable from both the Core and the Engine suites.
class SignalGenerator {

public:
    static std::vector<float> silence( uint32_t count );
    static std::vector<float> constant( float value, uint32_t count );
    static std::vector<float> ramp( uint32_t count );
    static std::vector<float> impulse( uint32_t count );

    static std::vector<float> sine( float frequency, uint32_t sampleRate, uint32_t count, float amplitude = 0.5f );

    // Fundamental plus decaying harmonics: closer to a plucked string than a
    // pure sine, and the case where a pitch detector can pick the wrong octave.
    static std::vector<float> pluckedString( float frequency, uint32_t sampleRate, uint32_t count, float amplitude = 0.5f );

    static std::vector<float> whiteNoise( uint32_t count, uint32_t seed = 1, float amplitude = 0.5f );

    static void addDcOffset( std::vector<float>& samples, float offset );

    static float peak( const std::vector<float>& samples );
    static float rms( const std::vector<float>& samples );

    // Standard tuning, low to high
    static constexpr float STANDARD_TUNING[ 6 ] = { 82.41f, 110.00f, 146.83f, 196.00f, 246.94f, 329.63f };
};

} // namespace TestSupport
