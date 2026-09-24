#pragma once

#include <cstdint>
#include <string_view>

#include <Core/Utils/AtomicValue.h>

namespace Core {

class DSPModule {

public:
    virtual ~DSPModule() = default;

    // Audio thread only — never allocate memory here
    virtual void process( float* left, float* right, uint32_t frames ) = 0;

    // Called outside the audio thread (setup / sample-rate change)
    virtual void prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    }

    virtual void reset() {
    }

    virtual void setParameter( std::string_view name, float value ) {
    }

    virtual float parameter( std::string_view name ) const {
        return 0.0f;
    }

    virtual std::string_view typeName() const = 0;

    bool isEnabled() const {
        return _enabled.load();
    }

    void setEnabled( bool enabled ) {
        _enabled.store( enabled );
    }

private:
    AtomicValue<bool> _enabled{ true };
};

} // namespace Core
