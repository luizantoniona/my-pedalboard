#include "EffectNode.h"

namespace Engine {

EffectNode::EffectNode( std::unique_ptr<DSPModule> module ) : _module( std::move( module ) ) {
}

void EffectNode::process( AudioBuffer& buffer ) {
    _module->process( buffer.left, buffer.right, buffer.frames );
}

DSPModule& EffectNode::module() {
    return *_module;
}

} // namespace Engine
