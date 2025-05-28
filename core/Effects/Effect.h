#pragma once

#include <cstddef>

class Effect {
public:
    virtual ~Effect() = default;

    virtual void process( float* buffer, size_t numFrames ) = 0;

    virtual const char* name() const = 0;
};
