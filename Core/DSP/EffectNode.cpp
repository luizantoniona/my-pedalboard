#include <Core/DSP/EffectNode.h>

namespace Core {

EffectNode::EffectNode( std::unique_ptr<DSPModule> effect ) :
    _effect( std::move( effect ) ) {
}

void EffectNode::process( AudioBuffer& buffer ) {
    if ( !_effect || !_effect->isEnabled() ) {
        return;
    }
    _effect->process( buffer.left(), buffer.isStereo() ? buffer.right() : nullptr, buffer.frames() );
}

void EffectNode::prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    if ( _effect ) {
        _effect->prepare( sampleRate, maxFrames );
    }
}

void EffectNode::reset() {
    if ( _effect ) {
        _effect->reset();
    }
}

} // namespace Core
