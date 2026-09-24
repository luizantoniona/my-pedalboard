#include <Core/Project/NodeRegistry.h>

#include <algorithm>

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
        created.registerType( "Gain", [] {
            return std::make_unique<Gain>();
        } );
        return created;
    }();
    return registry;
}

} // namespace Core
