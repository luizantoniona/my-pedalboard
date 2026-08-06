#pragma once

#include "AudioNode.h"
#include "DSPModule.h"

#include <memory>

namespace Engine {

class EffectNode : public AudioNode {
public:
    explicit EffectNode( std::unique_ptr<DSPModule> module );
    void process( AudioBuffer& buffer ) override;
    DSPModule& module();

private:
    std::unique_ptr<DSPModule> _module;
};

} // namespace Engine
