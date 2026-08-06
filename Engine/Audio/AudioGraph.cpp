#include "AudioGraph.h"

#include <algorithm>
#include <unordered_map>

namespace Engine {

AudioGraph::AudioGraph() = default;
AudioGraph::~AudioGraph() = default;

void AudioGraph::addNode( const std::string& id, std::shared_ptr<AudioNode> node ) {
    node->setId( id );
    _nodes[ id ] = std::move( node );
    rebuildExecutionOrder();
}

void AudioGraph::removeNode( const std::string& id ) {
    _nodes.erase( id );
    std::erase_if( _edges, [ &id ]( const Edge& e ) {
        return e.from == id || e.to == id;
    } );
    rebuildExecutionOrder();
}

void AudioGraph::connect( const std::string& fromId, const std::string& toId ) {
    _edges.push_back( { fromId, toId } );
    rebuildExecutionOrder();
}

void AudioGraph::disconnect( const std::string& fromId, const std::string& toId ) {
    std::erase_if( _edges, [ & ]( const Edge& e ) {
        return e.from == fromId && e.to == toId;
    } );
    rebuildExecutionOrder();
}

void AudioGraph::process( AudioBuffer& buffer ) {
    auto order = _executionOrder.load();
    if ( !order )
        return;

    for ( auto& node : *order ) {
        node->process( buffer );
    }
}

void AudioGraph::rebuildExecutionOrder() {
    _executionOrder.store(
        std::make_shared<const NodeList>( topologicalSort() ) );
}

// Kahn's algorithm
AudioGraph::NodeList AudioGraph::topologicalSort() const {
    std::unordered_map<std::string, int> inDegree;
    for ( auto& [ id, _ ] : _nodes )
        inDegree[ id ] = 0;
    for ( auto& edge : _edges )
        inDegree[ edge.to ]++;

    std::vector<std::string> queue;
    for ( auto& [ id, deg ] : inDegree ) {
        if ( deg == 0 )
            queue.push_back( id );
    }

    NodeList result;
    while ( !queue.empty() ) {
        auto id = queue.back();
        queue.pop_back();

        if ( auto it = _nodes.find( id ); it != _nodes.end() ) {
            result.push_back( it->second );
        }
        for ( auto& edge : _edges ) {
            if ( edge.from == id && --inDegree[ edge.to ] == 0 ) {
                queue.push_back( edge.to );
            }
        }
    }
    return result;
}

} // namespace Engine
