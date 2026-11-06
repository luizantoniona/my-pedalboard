#pragma once

#include <cstdint>

#include <Core/Utils/AtomicValue.h>

namespace Core {

class LevelMeter {

public:
    void prepare( uint32_t sampleRate );
    void process( const float* samples, uint32_t frames );
    void reset();

    float peak() const {
        return _peak.load();
    }

    bool isClipping() const {
        return _clipping.load();
    }

    void clearClipping() {
        _clipping.store( false );
    }

private:
    AtomicValue<float> _peak{ 0.0f };
    AtomicValue<bool> _clipping{ false };
    float _decayPerBlock{ 0.9f };
};

} // namespace Core
