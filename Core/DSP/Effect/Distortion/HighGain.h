#pragma once

#include <Core/DSP/DSPModule.h>
#include <Core/DSP/Effect/Filter/Biquad.h>
#include <Core/Utils/AtomicValue.h>
#include <Core/Utils/SmoothedValue.h>

namespace Core {

class HighGain : public DSPModule {

public:
    void process( float* left, float* right, uint32_t frames ) override;
    void prepare( uint32_t sampleRate, uint32_t maxFrames ) override;
    void reset() override;

    void setParameter( std::string_view name, float value ) override;
    float parameter( std::string_view name ) const override;

    std::span<const ParameterInfo> parameters() const override;

    std::string_view typeName() const override {
        return "HighGain";
    }

private:
    static constexpr int TONE_VALUE_COUNT = 4;

    class Channel {
    public:
        Biquad low;
        Biquad mid;
        Biquad high;
        float inputHighpassState{ 0.0f };
        float interstageState{ 0.0f };
        float outputState{ 0.0f };
    };

    float shaped( Channel& channel, float sample, float preGain, float level ) const;

    void redesignTone( const float* toneSnapshot );

    Channel _left;
    Channel _right;

    SmoothedValue _preGainSmoothed;
    SmoothedValue _levelSmoothed;

    AtomicValue<float> _distortion{ 0.7f };
    AtomicValue<float> _preGainLinear{ 13.8f };
    AtomicValue<float> _levelLinear{ 0.5f };

    AtomicValue<float> _lowDb{ 0.0f };
    AtomicValue<float> _midDb{ 0.0f };
    AtomicValue<float> _midFrequencyHz{ 800.0f };
    AtomicValue<float> _highDb{ 0.0f };

    AtomicValue<uint32_t> _sampleRate{ 48000 };

    float _appliedTone[ TONE_VALUE_COUNT ]{};

    float _inputHighpassCoefficient{ 0.0f };
    float _interstageCoefficient{ 1.0f };
    float _outputCoefficient{ 1.0f };

    bool _hasAppliedTone{ false };
};

} // namespace Core
