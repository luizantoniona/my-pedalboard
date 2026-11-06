#pragma once

#include <vector>

#include <Core/DSP/DSPModule.h>
#include <Core/Utils/AtomicValue.h>
#include <Core/Utils/SmoothedValue.h>

namespace Core {

class Reverb : public DSPModule {

public:
    void process( float* left, float* right, uint32_t frames ) override;
    void prepare( uint32_t sampleRate, uint32_t maxFrames ) override;
    void reset() override;

    void setParameter( std::string_view name, float value ) override;
    float parameter( std::string_view name ) const override;

    std::span<const ParameterInfo> parameters() const override;

    std::string_view typeName() const override {
        return "Reverb";
    }

private:
    static constexpr int COMB_COUNT = 8;
    static constexpr int ALLPASS_COUNT = 4;
    static constexpr int CHANNEL_COUNT = 2;

    class DelayLine {
    public:
        std::vector<float> buffer;
        float filterState{ 0.0f };
        uint32_t index{ 0 };
    };

    float combed( DelayLine& line, float input, float feedback, float damping ) const;
    float allpassed( DelayLine& line, float input ) const;

    float tank( int channel, float input, float feedback, float damping );

    DelayLine _combs[ CHANNEL_COUNT ][ COMB_COUNT ];
    DelayLine _allpasses[ CHANNEL_COUNT ][ ALLPASS_COUNT ];

    SmoothedValue _mixSmoothed;

    AtomicValue<float> _size{ 0.5f };
    AtomicValue<float> _damping{ 0.5f };
    AtomicValue<float> _mix{ 0.25f };
};

} // namespace Core
