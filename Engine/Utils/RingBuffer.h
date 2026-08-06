#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <optional>

namespace Engine {

// Single-producer single-consumer lock-free ring buffer.
// Safe to use between exactly two threads (e.g. audio thread ↔ UI thread).
template <typename T, std::size_t Capacity>
class RingBuffer {
    static_assert( ( Capacity & ( Capacity - 1 ) ) == 0, "Capacity must be a power of two" );

public:
    bool push( const T& value ) {
        const std::size_t head = _head.load( std::memory_order_relaxed );
        const std::size_t next = ( head + 1 ) & _mask;

        if ( next == _tail.load( std::memory_order_acquire ) )
            return false; // full

        _buffer[ head ] = value;
        _head.store( next, std::memory_order_release );
        return true;
    }

    std::optional<T> pop() {
        const std::size_t tail = _tail.load( std::memory_order_relaxed );

        if ( tail == _head.load( std::memory_order_acquire ) )
            return std::nullopt; // empty

        T value = _buffer[ tail ];
        _tail.store( ( tail + 1 ) & _mask, std::memory_order_release );
        return value;
    }

    bool isEmpty() const {
        return _head.load( std::memory_order_acquire ) ==
               _tail.load( std::memory_order_acquire );
    }

private:
    static constexpr std::size_t _mask = Capacity - 1;

    std::array<T, Capacity> _buffer{};
    std::atomic<std::size_t> _head{ 0 };
    std::atomic<std::size_t> _tail{ 0 };
};

} // namespace Engine
