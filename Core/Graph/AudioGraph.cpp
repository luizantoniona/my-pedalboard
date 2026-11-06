#include <Core/Graph/AudioGraph.h>

#include <algorithm>

namespace Core {

AudioGraph::AudioGraph() = default;

AudioGraph::~AudioGraph() {
    delete _executionOrder.exchange( nullptr );
}

void AudioGraph::addNode( const std::string& id, std::shared_ptr<AudioNode> node ) {
    if ( !node ) {
        return;
    }
    node->setId( id );
    node->prepare( _sampleRate, _maxFrames );
    _nodes[ id ] = std::move( node );
    rebuildExecutionOrder();
}

void AudioGraph::removeNode( const std::string& id ) {
    _nodes.erase( id );
    std::erase_if( _edges, [ &id ]( const Edge& edge ) {
        return edge.from == id || edge.to == id;
    } );
    rebuildExecutionOrder();
}

void AudioGraph::connect( const std::string& fromId, const std::string& toId ) {
    const bool exists = std::any_of( _edges.begin(), _edges.end(), [ & ]( const Edge& edge ) {
        return edge.from == fromId && edge.to == toId;
    } );
    if ( exists ) {
        return;
    }
    _edges.push_back( Edge{ fromId, toId } );
    rebuildExecutionOrder();
}

void AudioGraph::disconnect( const std::string& fromId, const std::string& toId ) {
    std::erase_if( _edges, [ & ]( const Edge& edge ) {
        return edge.from == fromId && edge.to == toId;
    } );
    rebuildExecutionOrder();
}

void AudioGraph::setSerialChain( const std::vector<std::string>& orderedIds ) {
    _edges.clear();

    const std::string* previous = nullptr;
    for ( const std::string& id : orderedIds ) {
        if ( _nodes.count( id ) == 0 ) {
            continue;
        }
        if ( previous != nullptr ) {
            _edges.push_back( Edge{ *previous, id } );
        }
        previous = &id;
    }

    rebuildExecutionOrder();
}

void AudioGraph::clear() {
    _nodes.clear();
    _edges.clear();
    rebuildExecutionOrder();
}

void AudioGraph::prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    _sampleRate = sampleRate;
    _maxFrames = maxFrames;
    for ( auto& [ id, node ] : _nodes ) {
        node->prepare( sampleRate, maxFrames );
    }
}

void AudioGraph::process( AudioBuffer& buffer ) {
    const NodeList* order = _executionOrder.load( std::memory_order_acquire );
    if ( order != nullptr ) {
        for ( AudioNode* node : *order ) {
            node->process( buffer );
        }
    }
    _processGeneration.fetch_add( 1, std::memory_order_release );
}

void AudioGraph::rebuildExecutionOrder() {
    collectGarbage();

    auto fresh = std::make_unique<NodeList>( topologicalSort() );
    const NodeList* previous = _executionOrder.exchange( fresh.release(), std::memory_order_acq_rel );

    if ( previous != nullptr ) {
        _retired.push_back( RetiredList{
            std::unique_ptr<NodeList>( const_cast<NodeList*>( previous ) ),
            _processGeneration.load( std::memory_order_acquire ) } );
    }
}

void AudioGraph::collectGarbage() {
    const uint64_t current = _processGeneration.load( std::memory_order_acquire );
    std::erase_if( _retired, [ current ]( const RetiredList& retired ) {
        return current > retired.generation + 1;
    } );
}

AudioGraph::NodeList AudioGraph::topologicalSort() const {
    std::unordered_map<std::string, int> inDegree;
    inDegree.reserve( _nodes.size() );
    for ( const auto& [ id, node ] : _nodes ) {
        inDegree[ id ] = 0;
    }
    for ( const Edge& edge : _edges ) {
        if ( inDegree.count( edge.to ) > 0 && inDegree.count( edge.from ) > 0 ) {
            inDegree[ edge.to ]++;
        }
    }

    std::vector<std::string> ready;
    for ( const auto& [ id, degree ] : inDegree ) {
        if ( degree == 0 ) {
            ready.push_back( id );
        }
    }
    std::sort( ready.begin(), ready.end() );

    NodeList sorted;
    sorted.reserve( _nodes.size() );

    while ( !ready.empty() ) {
        const std::string id = ready.front();
        ready.erase( ready.begin() );

        const auto found = _nodes.find( id );
        if ( found != _nodes.end() ) {
            sorted.push_back( found->second.get() );
        }

        std::vector<std::string> unlocked;
        for ( const Edge& edge : _edges ) {
            if ( edge.from != id ) {
                continue;
            }
            const auto target = inDegree.find( edge.to );
            if ( target != inDegree.end() && --target->second == 0 ) {
                unlocked.push_back( edge.to );
            }
        }
        std::sort( unlocked.begin(), unlocked.end() );
        ready.insert( ready.end(), unlocked.begin(), unlocked.end() );
    }

    return sorted;
}

} // namespace Core
