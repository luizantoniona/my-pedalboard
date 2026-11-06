#pragma once

#include <string>

#include <Core/DSP/AudioBuffer.h>

namespace Core {

class AudioNode {

public:
    virtual ~AudioNode() = default;

    virtual void process( AudioBuffer& buffer ) = 0;

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
