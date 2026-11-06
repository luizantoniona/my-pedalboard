#pragma once

#include <Core/DSP/DSPModule.h>
#include <Core/DSP/Effect/Filter/Biquad.h>
#include <Core/Utils/AtomicValue.h>
#include <Core/Utils/SmoothedValue.h>

namespace Core {

class Equalizer : public DSPModule {

public:
    void process( float* left, float* right, uint32_t frames ) override;
    void prepare( uint32_t sampleRate, uint32_t maxFrames ) override;
    void reset() override;

    void setParameter( std::string_view name, float value ) override;
    float parameter( std::string_view name ) const override;

    std::span<const ParameterInfo> parameters() const override;

    std::string_view typeName() const override {
        return "Equalizer";
    }

private:
    static constexpr int BAND_COUNT = 7;

    void redesignBands( const float* gainSnapshot );

    Biquad _left[ BAND_COUNT ];
    Biquad _right[ BAND_COUNT ];

    SmoothedValue _levelSmoothed;

    AtomicValue<float> _gainsDb[ BAND_COUNT ]{
        AtomicValue<float>( 0.0f ),
        AtomicValue<float>( 0.0f ),
        AtomicValue<float>( 0.0f ),
        AtomicValue<float>( 0.0f ),
        AtomicValue<float>( 0.0f ),
        AtomicValue<float>( 0.0f ),
        AtomicValue<float>( 0.0f ),
    };

    AtomicValue<float> _levelLinear{ 1.0f };

    float _appliedGainsDb[ BAND_COUNT ]{};

    AtomicValue<uint32_t> _sampleRate{ 48000 };

    bool _hasApplied{ false };
};

} // namespace Core
