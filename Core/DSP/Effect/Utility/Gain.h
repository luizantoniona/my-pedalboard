#pragma once

#include <Core/DSP/DSPModule.h>
#include <Core/Utils/AtomicValue.h>
#include <Core/Utils/SmoothedValue.h>

namespace Core {

class Gain : public DSPModule {

public:
    void process( float* left, float* right, uint32_t frames ) override;
    void prepare( uint32_t sampleRate, uint32_t maxFrames ) override;
    void reset() override;

    void setParameter( std::string_view name, float value ) override;
    float parameter( std::string_view name ) const override;

    std::string_view typeName() const override {
        return "Gain";
    }

private:
    // Linear scale; exposed as the "gainDb" parameter
    AtomicValue<float> _gain{ 1.0f };
    SmoothedValue _smoothed;
};

} // namespace Core
