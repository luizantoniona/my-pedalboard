#pragma once

#include <cstdint>
#include <string>

namespace Engine {

struct AudioBuffer {
    float* left;
    float* right;
    uint32_t frames;
    uint32_t sampleRate;
};

class AudioNode {
public:
    virtual ~AudioNode() = default;
    virtual void process( AudioBuffer& buffer ) = 0;

    const std::string& id() const {
        return _id;
    }
    void setId( std::string id ) {
        _id = std::move( id );
    }

private:
    std::string _id;
};

} // namespace Engine
