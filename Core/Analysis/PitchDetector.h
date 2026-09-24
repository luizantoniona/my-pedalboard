#pragma once

#include <cstdint>
#include <vector>

namespace Core {

// YIN pitch detection. Runs outside the audio thread — the audio thread only
// pushes samples into a RingBuffer, a timer drains them and calls analyze().
//
// Input is decimated to roughly 12 kHz before analysis: a guitar fundamental
// never exceeds ~1.3 kHz, and the decimation cuts the cost of the difference
// function by the square of the factor.
class PitchDetector {

public:
    class Result {

    public:
        bool voiced{ false };
        float frequency{ 0.0f };
        float clarity{ 0.0f };
    };

    void prepare( uint32_t sampleRate );
    Result analyze( const float* samples, uint32_t count );

    // Number of input samples analyze() expects
    uint32_t windowSize() const {
        return _inputSize;
    }

private:
    void decimate( const float* samples );
    void computeDifference();
    void computeCumulativeMean();
    uint32_t absoluteThreshold() const;
    float parabolicInterpolation( uint32_t tau ) const;

    std::vector<float> _working;
    std::vector<float> _difference;
    std::vector<float> _cumulative;

    uint32_t _sampleRate{ 48000 };
    uint32_t _workingRate{ 12000 };
    uint32_t _inputSize{ 2048 };
    uint32_t _decimation{ 4 };
    uint32_t _tauMin{ 10 };
    uint32_t _tauMax{ 256 };

    static constexpr float THRESHOLD = 0.15f;
    static constexpr float SILENCE_RMS = 0.003f;
};

} // namespace Core
