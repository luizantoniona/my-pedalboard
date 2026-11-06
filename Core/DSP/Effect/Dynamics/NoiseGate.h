#pragma once

#include <Core/DSP/DSPModule.h>
#include <Core/Utils/AtomicValue.h>

namespace Core {

class NoiseGate : public DSPModule {

public:
    void process( float* left, float* right, uint32_t frames ) override;
    void prepare( uint32_t sampleRate, uint32_t maxFrames ) override;
    void reset() override;

    void setParameter( std::string_view name, float value ) override;
    float parameter( std::string_view name ) const override;

    std::span<const ParameterInfo> parameters() const override;

    std::string_view typeName() const override {
        return "NoiseGate";
    }

private:
    void refreshCoefficients();

    AtomicValue<float> _threshold{ 0.001f };
    AtomicValue<float> _attackMs{ 2.0f };
    AtomicValue<float> _releaseMs{ 120.0f };
    AtomicValue<float> _holdMs{ 40.0f };

    AtomicValue<float> _attackCoefficient{ 0.0f };
    AtomicValue<float> _releaseCoefficient{ 0.0f };
    AtomicValue<float> _detectorCoefficient{ 0.0f };
    AtomicValue<int> _holdSamples{ 0 };

    AtomicValue<uint32_t> _sampleRate{ 48000 };

    float _envelope{ 0.0f };
    float _gateGain{ 0.0f };
    int _holdCounter{ 0 };
};

} // namespace Core
