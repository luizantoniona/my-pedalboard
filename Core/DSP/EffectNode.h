#pragma once

#include <memory>

#include <Core/DSP/AudioNode.h>
#include <Core/DSP/DSPModule.h>

namespace Core {

class EffectNode : public AudioNode {

public:
    explicit EffectNode( std::unique_ptr<DSPModule> effect );

    void process( AudioBuffer& buffer ) override;
    void prepare( uint32_t sampleRate, uint32_t maxFrames ) override;
    void reset() override;

    DSPModule* effect() const {
        return _effect.get();
    }

private:
    std::unique_ptr<DSPModule> _effect;
};

} // namespace Core
