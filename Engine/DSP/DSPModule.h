#pragma once

#include <cstdint>
#include <string_view>

namespace Engine {

class DSPModule {
public:
    virtual ~DSPModule() = default;

    // Called exclusively on the audio thread — never allocate memory here
    virtual void process( float* left, float* right, uint32_t frames ) = 0;

    // Called outside the audio thread (setup / sample-rate change)
    virtual void prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    }
    virtual void reset() {
    }

    // Named parameter access — enables presets and automation
    virtual void setParameter( std::string_view name, float value ) {
    }
    virtual float getParameter( std::string_view name ) const {
        return 0.0f;
    }

    virtual std::string_view typeName() const = 0;
};

} // namespace Engine
