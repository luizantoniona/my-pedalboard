#pragma once

#include <Engine/DSP/DSPModule.h>
#include <Engine/Utils/AtomicValue.h>
#include <Engine/Utils/AudioMath.h>

namespace Engine {

class Gain : public DSPModule {
public:
    Gain() = default;

    void process( float* left, float* right, uint32_t frames ) override;
    void setParameter( std::string_view name, float value ) override;
    float getParameter( std::string_view name ) const override;
    std::string_view typeName() const override {
        return "Gain";
    }

private:
    // Stored in linear scale; exposed via "gainDb" parameter
    AtomicValue<float> _gain{ 1.0f };
};

} // namespace Engine
