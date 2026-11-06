#pragma once

#include <cstdint>
#include <span>
#include <string_view>

#include <Core/DSP/ParameterInfo.h>
#include <Core/Utils/AtomicValue.h>

namespace Core {

class DSPModule {

public:
    virtual ~DSPModule() = default;

    virtual void process( float* left, float* right, uint32_t frames ) = 0;

    virtual void prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    }

    virtual void reset() {
    }

    virtual void setParameter( std::string_view name, float value ) {
    }

    virtual float parameter( std::string_view name ) const {
        return 0.0f;
    }

    virtual std::span<const ParameterInfo> parameters() const {
        return {};
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
