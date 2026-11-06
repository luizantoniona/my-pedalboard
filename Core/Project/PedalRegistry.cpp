#include <Core/Project/PedalRegistry.h>

#include <Core/Project/NodeRegistry.h>

namespace Core {

PedalRegistry::PedalRegistry( const NodeRegistry& nodes ) :
    _nodes( nodes ) {
}

void PedalRegistry::registerPedal( PedalDefinition definition ) {
    const std::string id = definition.id;
    if ( _definitions.count( id ) == 0 ) {
        _order.push_back( id );
    }
    _definitions[ id ] = std::move( definition );
}

const PedalDefinition* PedalRegistry::definition( const std::string& id ) const {
    const auto found = _definitions.find( id );
    return found == _definitions.end() ? nullptr : &found->second;
}

std::vector<std::string> PedalRegistry::ids() const {
    return _order;
}

std::unique_ptr<DSPModule> PedalRegistry::create( const std::string& id ) const {
    const PedalDefinition* pedal = definition( id );
    if ( pedal == nullptr ) {
        return nullptr;
    }

    std::unique_ptr<DSPModule> module = _nodes.create( pedal->algorithm );
    if ( !module ) {
        return nullptr;
    }

    for ( const ParameterInfo& info : module->parameters() ) {
        module->setParameter( info.name, info.defaultValue );
    }
    for ( const ParameterOverride& entry : pedal->overrides ) {
        module->setParameter( entry.name, entry.value );
    }
    return module;
}

PedalRegistry& PedalRegistry::withBuiltinPedals() {
    static PedalRegistry registry = [] {
        PedalRegistry created( NodeRegistry::withBuiltinTypes() );
        created.registerPedal( { "Gain", "Gain", "Gain", {} } );
        created.registerPedal( { "NoiseGate", "Noise Gate", "NoiseGate", {} } );
        created.registerPedal( { "Overdrive", "Overdrive", "Overdrive", {} } );
        created.registerPedal( { "HighGain", "High Gain", "HighGain", {} } );
        created.registerPedal( { "Equalizer", "Equalizer", "Equalizer", {} } );
        created.registerPedal( { "Chorus", "Chorus", "Chorus", {} } );
        created.registerPedal( { "Delay", "Delay", "Delay", {} } );
        created.registerPedal( { "SlapbackDelay",
                                 "Slapback Delay",
                                 "Delay",
                                 { { "time", 90.0f }, { "feedback", 0.1f }, { "tone", 0.7f }, { "mix", 0.35f } } } );
        created.registerPedal( { "Reverb", "Reverb", "Reverb", {} } );
        created.registerPedal(
            { "RoomReverb", "Room Reverb", "Reverb", { { "size", 0.3f }, { "damping", 0.6f }, { "mix", 0.2f } } } );
        created.registerPedal(
            { "HallReverb", "Hall Reverb", "Reverb", { { "size", 0.85f }, { "damping", 0.35f }, { "mix", 0.3f } } } );
        return created;
    }();
    return registry;
}

} // namespace Core
