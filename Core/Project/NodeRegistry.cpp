#include <Core/Project/NodeRegistry.h>

#include <algorithm>

#include <Core/DSP/Effect/Delay/Delay.h>
#include <Core/DSP/Effect/Distortion/HighGain.h>
#include <Core/DSP/Effect/Distortion/Overdrive.h>
#include <Core/DSP/Effect/Dynamics/NoiseGate.h>
#include <Core/DSP/Effect/Filter/Equalizer.h>
#include <Core/DSP/Effect/Modulation/Chorus.h>
#include <Core/DSP/Effect/Reverb/Reverb.h>
#include <Core/DSP/Effect/Utility/Gain.h>

namespace Core {

void NodeRegistry::registerType( std::string typeName, Creator creator ) {
    _creators[ std::move( typeName ) ] = std::move( creator );
}

std::unique_ptr<DSPModule> NodeRegistry::create( const std::string& typeName ) const {
    const auto found = _creators.find( typeName );
    if ( found == _creators.end() ) {
        return nullptr;
    }
    return found->second();
}

bool NodeRegistry::hasType( const std::string& typeName ) const {
    return _creators.count( typeName ) > 0;
}

std::vector<std::string> NodeRegistry::typeNames() const {
    std::vector<std::string> names;
    names.reserve( _creators.size() );
    for ( const auto& [ name, creator ] : _creators ) {
        names.push_back( name );
    }
    std::sort( names.begin(), names.end() );
    return names;
}

NodeRegistry& NodeRegistry::withBuiltinTypes() {
    static NodeRegistry registry = [] {
        NodeRegistry created;
        created.registerType( "Chorus", [] {
            return std::make_unique<Chorus>();
        } );
        created.registerType( "Delay", [] {
            return std::make_unique<Delay>();
        } );
        created.registerType( "Equalizer", [] {
            return std::make_unique<Equalizer>();
        } );
        created.registerType( "Gain", [] {
            return std::make_unique<Gain>();
        } );
        created.registerType( "HighGain", [] {
            return std::make_unique<HighGain>();
        } );
        created.registerType( "NoiseGate", [] {
            return std::make_unique<NoiseGate>();
        } );
        created.registerType( "Overdrive", [] {
            return std::make_unique<Overdrive>();
        } );
        created.registerType( "Reverb", [] {
            return std::make_unique<Reverb>();
        } );
        return created;
    }();
    return registry;
}

} // namespace Core
