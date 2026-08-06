#pragma once

#include <atomic>

namespace Engine {

// Thread-safe parameter wrapper for use across UI thread (write) and audio thread (read).
// Uses relaxed ordering — suitable for continuously-updated float parameters (gain, freq, etc.)
// where occasional stale reads are acceptable.
template <typename T>
class AtomicValue {
public:
    explicit AtomicValue( T initial = T{} ) : _value( initial ) {
    }

    void store( T value ) noexcept {
        _value.store( value, std::memory_order_relaxed );
    }

    T load() const noexcept {
        return _value.load( std::memory_order_relaxed );
    }

    // Convenience: assign from UI thread
    AtomicValue& operator=( T value ) noexcept {
        store( value );
        return *this;
    }

    // Convenience: read from audio thread
    operator T() const noexcept {
        return load();
    }

private:
    std::atomic<T> _value;
};

} // namespace Engine
