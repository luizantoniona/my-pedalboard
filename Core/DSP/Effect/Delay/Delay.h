#pragma once

#include <vector>

#include <Core/DSP/DSPModule.h>
#include <Core/Utils/AtomicValue.h>
#include <Core/Utils/SmoothedValue.h>

namespace Core {

class Delay : public DSPModule {

public:
    void process( float* left, float* right, uint32_t frames ) override;
    void prepare( uint32_t sampleRate, uint32_t maxFrames ) override;
    void reset() override;

    void setParameter( std::string_view name, float value ) override;
    float parameter( std::string_view name ) const override;

    std::span<const ParameterInfo> parameters() const override;

    std::string_view typeName() const override {
        return "Delay";
    }

private:
    class Channel {
    public:
        std::vector<float> line;
        float feedbackToneState{ 0.0f };
    };

    float echoed( Channel& channel, float sample, uint32_t writeIndex, float delaySamples, float feedback, float toneCoefficient ) const;

    void refreshTone();

    Channel _left;
    Channel _right;

    SmoothedValue _timeSmoothed;
    SmoothedValue _mixSmoothed;

    AtomicValue<float> _timeMilliseconds{ 380.0f };
    AtomicValue<float> _feedback{ 0.35f };
    AtomicValue<float> _tone{ 0.6f };
    AtomicValue<float> _toneCoefficient{ 1.0f };
    AtomicValue<float> _mix{ 0.3f };

    AtomicValue<uint32_t> _sampleRate{ 48000 };

    uint32_t _writeIndex{ 0 };
};

} // namespace Core
