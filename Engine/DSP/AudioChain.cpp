#include "AudioChain.h"

namespace Engine {

void AudioChain::addEffect( std::shared_ptr<AudioEffect> effect ) {
    _effects.push_back( effect );
}

void AudioChain::process( float* left, float* right, unsigned int frames ) {
    for ( auto& effect : _effects ) {
        effect->process( left, right, frames );
    }
}

} // namespace Engine
