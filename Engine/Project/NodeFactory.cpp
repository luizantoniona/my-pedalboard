#include "NodeFactory.h"

#include <QDebug>

namespace Engine {

void NodeFactory::registerType( std::string typeName, Creator creator ) {
    _creators[ std::move( typeName ) ] = std::move( creator );
}

std::unique_ptr<DSPModule> NodeFactory::create( const std::string& typeName ) const {
    if ( auto it = _creators.find( typeName ); it != _creators.end() )
        return it->second();
    qWarning() << "NodeFactory: unknown type" << QString::fromStdString( typeName );
    return nullptr;
}

bool NodeFactory::hasType( const std::string& typeName ) const {
    return _creators.contains( typeName );
}

} // namespace Engine
