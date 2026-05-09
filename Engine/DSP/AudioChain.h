#ifndef AUDIOCHAIN_H
#define AUDIOCHAIN_H

#include <memory>
#include <vector>

#include "AudioEffect.h"

namespace Engine {

class AudioChain {
public:
    void addEffect( std::shared_ptr<AudioEffect> effect );

    void process( float* left, float* right, unsigned int frames );

private:
    std::vector<std::shared_ptr<AudioEffect>> _effects;
};

} // namespace Engine

#endif
