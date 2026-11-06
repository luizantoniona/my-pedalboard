#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <optional>

namespace Core {

template <typename T, std::size_t Capacity>
class RingBuffer {
    static_assert( ( Capacity & ( Capacity - 1 ) ) == 0, "Capacity must be a power of two" );

public:
    bool push( const T& value ) {
        const std::size_t head = _head.load( std::memory_order_relaxed );
        const std::size_t next = ( head + 1 ) & MASK;

        if ( next == _tail.load( std::memory_order_acquire ) ) {
            return false;
        }

        _buffer[ head ] = value;
        _head.store( next, std::memory_order_release );
        return true;
    }

    std::optional<T> pop() {
        const std::size_t tail = _tail.load( std::memory_order_relaxed );
        if ( tail == _head.load( std::memory_order_acquire ) ) {
            return std::nullopt;
        }

        T value = _buffer[ tail ];
        _tail.store( ( tail + 1 ) & MASK, std::memory_order_release );
        return value;
    }

    std::size_t writeBlock( const T* values, std::size_t count ) {
        std::size_t written = 0;
        while ( written < count && push( values[ written ] ) ) {
            written++;
        }
        return written;
    }

    std::size_t readBlock( T* destination, std::size_t count ) {
        std::size_t read = 0;
        while ( read < count ) {
            const std::optional<T> value = pop();
            if ( !value.has_value() ) {
                break;
            }
            destination[ read ] = *value;
            read++;
        }
        return read;
    }

    std::size_t available() const {
        const std::size_t head = _head.load( std::memory_order_acquire );
        const std::size_t tail = _tail.load( std::memory_order_acquire );
        return ( head - tail ) & MASK;
    }

    void clear() {
        _tail.store( _head.load( std::memory_order_acquire ), std::memory_order_release );
    }

    static constexpr std::size_t capacity() {
        return Capacity - 1;
    }

private:
    static constexpr std::size_t MASK = Capacity - 1;

    std::array<T, Capacity> _buffer{};
    std::atomic<std::size_t> _head{ 0 };
    std::atomic<std::size_t> _tail{ 0 };
};

} // namespace Core
