#pragma once

#include <atomic>

namespace Core {

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
