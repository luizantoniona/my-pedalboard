#pragma once

#include <vector>

#include <Core/DSP/DSPModule.h>
#include <Core/Utils/AtomicValue.h>
#include <Core/Utils/SmoothedValue.h>

namespace Core {

class Chorus : public DSPModule {

public:
    void process( float* left, float* right, uint32_t frames ) override;
    void prepare( uint32_t sampleRate, uint32_t maxFrames ) override;
    void reset() override;

    void setParameter( std::string_view name, float value ) override;
    float parameter( std::string_view name ) const override;

    std::span<const ParameterInfo> parameters() const override;

    std::string_view typeName() const override {
        return "Chorus";
    }

private:
    float modulated( const std::vector<float>& line, uint32_t writeIndex, float delaySamples ) const;

    float delaySamplesAt( float lfoPhase, float depth ) const;

    std::vector<float> _lineLeft;
    std::vector<float> _lineRight;

    SmoothedValue _mixSmoothed;

    AtomicValue<float> _rateHz{ 0.8f };
    AtomicValue<float> _depth{ 0.5f };
    AtomicValue<float> _mix{ 0.5f };

    AtomicValue<uint32_t> _sampleRate{ 48000 };

    float _lfoPhase{ 0.0f };

    uint32_t _writeIndex{ 0 };
};

} // namespace Core
