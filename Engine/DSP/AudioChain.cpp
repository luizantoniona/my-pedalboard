#include "AudioChain.h"

namespace Engine {

// TODO: Review this class;
// TODO: Is it the best way to wrap differents DSP modules?
// TODO: Define the real architecture;
// TODO: Pedal has multiples chains, or mutiples DSP modules?
// TODO: Chain has parameters?
// TODO: Review DSPModules too;
// TODO: Should them have internal parameters?
// TODO: Dynamic setted, or construct setted?

void AudioChain::addModule( std::shared_ptr<DSPModule> module ) {
    _modules.push_back( module );
}

void AudioChain::process( float* left, float* right, unsigned int frames ) {
    for ( auto& module : _modules ) {
        module->process( left, right, frames );
    }
}

} // namespace Engine
