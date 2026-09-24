#pragma once

#include <string>

#include <Core/DSP/AudioBuffer.h>

namespace Core {

class AudioNode {

public:
    virtual ~AudioNode() = default;

    // Audio thread only — never allocate, lock or block here
    virtual void process( AudioBuffer& buffer ) = 0;

    // Called outside the audio thread
    virtual void prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    }

    virtual void reset() {
    }

    const std::string& id() const {
        return _id;
    }

    void setId( std::string id ) {
        _id = std::move( id );
    }

private:
    std::string _id;
};

} // namespace Core
