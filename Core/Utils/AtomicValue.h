#pragma once

#include <atomic>

namespace Core {

// Parameter shared between the UI thread (write) and the audio thread (read).
// Uses relaxed ordering — suitable for continuously-updated float parameters (gain, freq, etc.)
// where occasional stale reads are acceptable.
template <typename T>
class AtomicValue {
public:
    explicit AtomicValue( T initial = T{} ) :
        _value( initial ) {
    }

    void store( T value ) noexcept {
        _value.store( value, std::memory_order_relaxed );
    }

    T load() const noexcept {
        return _value.load( std::memory_order_relaxed );
    }

    AtomicValue& operator=( T value ) noexcept {
        store( value );
        return *this;
    }

    operator T() const noexcept {
        return load();
    }

private:
    std::atomic<T> _value;
};

} // namespace Core
