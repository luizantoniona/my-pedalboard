#pragma once

#include <cmath>

#include <Core/DSP/DSPModule.h>
#include <Core/DSP/Effect/Filter/Biquad.h>
#include <Core/Utils/AtomicValue.h>
#include <Core/Utils/SmoothedValue.h>

namespace Core {

class Overdrive : public DSPModule {

public:
    void process( float* left, float* right, uint32_t frames ) override;
    void prepare( uint32_t sampleRate, uint32_t maxFrames ) override;
    void reset() override;

    void setParameter( std::string_view name, float value ) override;
    float parameter( std::string_view name ) const override;

    std::span<const ParameterInfo> parameters() const override;

    std::string_view typeName() const override {
        return "Overdrive";
    }

private:
    class Channel {
    public:
        Biquad hump;
        float inputHighpassState{ 0.0f };
        float toneState{ 0.0f };
        float dcState{ 0.0f };
    };

    static constexpr float ASYMMETRY_BIAS = 0.25f;

    float asymmetricClip( float sample ) const;

    float shaped( Channel& channel, float sample, float drive, float level, float mix, float toneCoefficient ) const;

    void refreshTone();

    Channel _left;
    Channel _right;

    SmoothedValue _driveSmoothed;
    SmoothedValue _levelSmoothed;
    SmoothedValue _mixSmoothed;

    AtomicValue<float> _drive{ 4.0f };
    AtomicValue<float> _levelLinear{ 0.5f };
    AtomicValue<float> _mix{ 1.0f };
    AtomicValue<float> _tone{ 0.5f };
    AtomicValue<float> _toneCoefficient{ 1.0f };

    AtomicValue<uint32_t> _sampleRate{ 48000 };

    float _tanhOfBias{ std::tanh( ASYMMETRY_BIAS ) };

    float _inputHighpassCoefficient{ 0.0f };
    float _dcCoefficient{ 0.0f };
};

} // namespace Core
