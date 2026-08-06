#include "Serializer.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

#include <Engine/DSP/EffectNode.h>

namespace Engine {

// ── Serializer ────────────────────────────────────────────────────────────────

Serializer::Serializer( NodeFactory& factory ) : _factory( factory ) {
}

bool Serializer::load( const QJsonObject& root, AudioGraph& graph ) const {
    const auto nodesArray = root[ "chain" ].toArray();
    const auto edgesArray = root[ "graph" ].toObject()[ "edges" ].toArray();

    // Create nodes
    for ( const auto& entry : nodesArray ) {
        const QJsonObject obj = entry.toObject();
        const std::string id = obj[ "id" ].toString().toStdString();
        const std::string type = obj[ "type" ].toString().toStdString();

        auto module = _factory.create( type );
        if ( !module )
            return false;

        // Restore parameters
        const QJsonObject params = obj[ "params" ].toObject();
        for ( auto it = params.begin(); it != params.end(); ++it ) {
            module->setParameter( it.key().toStdString(), static_cast<float>( it.value().toDouble() ) );
        }

        graph.addNode( id, std::make_shared<EffectNode>( std::move( module ) ) );
    }

    // Wire edges
    for ( const auto& entry : edgesArray ) {
        const QJsonObject obj = entry.toObject();
        graph.connect( obj[ "from" ].toString().toStdString(),
                       obj[ "to" ].toString().toStdString() );
    }

    return true;
}

QJsonObject Serializer::save( AudioGraph& graph ) const {
    QJsonArray nodesArray;
    for ( const auto& [ id, node ] : graph.nodes() ) {
        auto* effect = dynamic_cast<EffectNode*>( node.get() );
        if ( !effect )
            continue;

        QJsonObject params;
        // Subclasses can override getParameter for known names;
        // serialization of all params requires an enumeration API added later
        Q_UNUSED( effect )

        nodesArray.append( QJsonObject{
            { "id", QString::fromStdString( id ) },
            { "type", QString::fromStdString( std::string( effect->module().typeName() ) ) },
            { "params", params } } );
    }

    QJsonArray edgesArray;
    for ( const auto& edge : graph.edges() ) {
        edgesArray.append( QJsonObject{
            { "from", QString::fromStdString( edge.from ) },
            { "to", QString::fromStdString( edge.to ) } } );
    }

    return QJsonObject{
        { "chain", nodesArray },
        { "graph", QJsonObject{ { "edges", edgesArray } } } };
}

} // namespace Engine
