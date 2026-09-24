#pragma once

#include <cstdint>

namespace Core {

class AudioBuffer {

public:
    AudioBuffer( float* left, float* right, uint32_t frames, uint32_t sampleRate ) :
        _left( left ),
        _right( right ),
        _frames( frames ),
        _sampleRate( sampleRate ),
        _stereo( right != nullptr ) {
    }

    float* left() const {
        return _left;
    }

    float* right() const {
        return _right;
    }

    uint32_t frames() const {
        return _frames;
    }

    uint32_t sampleRate() const {
        return _sampleRate;
    }

    // A guitar source is mono until a stage opens it into stereo
    bool isStereo() const {
        return _stereo;
    }

    void openStereo() {
        _stereo = true;
    }

private:
    float* _left;
    float* _right;
    uint32_t _frames;
    uint32_t _sampleRate;
    bool _stereo;
};

} // namespace Core
