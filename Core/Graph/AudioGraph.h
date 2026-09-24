#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <Core/DSP/AudioNode.h>

namespace Core {

// Nodes are mutated from the engine thread and read from the audio thread.
// The execution order is published as a raw pointer to an immutable vector:
// the audio thread does one acquire load and walks raw pointers, with no
// reference counting. std::atomic<std::shared_ptr> is not lock-free on any
// current implementation, so it cannot be used here.
class AudioGraph {

public:
    class Edge {

    public:
        std::string from;
        std::string to;
    };

    AudioGraph();
    ~AudioGraph();

    AudioGraph( const AudioGraph& ) = delete;
    AudioGraph& operator=( const AudioGraph& ) = delete;

    void addNode( const std::string& id, std::shared_ptr<AudioNode> node );
    void removeNode( const std::string& id );
    void connect( const std::string& fromId, const std::string& toId );
    void disconnect( const std::string& fromId, const std::string& toId );
    void clear();

    void prepare( uint32_t sampleRate, uint32_t maxFrames );

    // Audio thread only
    void process( AudioBuffer& buffer );

    const std::unordered_map<std::string, std::shared_ptr<AudioNode>>& nodes() const {
        return _nodes;
    }

    const std::vector<Edge>& edges() const {
        return _edges;
    }

private:
    using NodeList = std::vector<AudioNode*>;

    class RetiredList {

    public:
        std::unique_ptr<NodeList> list;
        uint64_t generation;
    };

    void rebuildExecutionOrder();
    NodeList topologicalSort() const;
    void collectGarbage();

    std::unordered_map<std::string, std::shared_ptr<AudioNode>> _nodes;
    std::vector<Edge> _edges;
    std::vector<RetiredList> _retired;

    std::atomic<const NodeList*> _executionOrder{ nullptr };
    std::atomic<uint64_t> _processGeneration{ 0 };

    uint32_t _sampleRate{ 48000 };
    uint32_t _maxFrames{ 512 };
};

} // namespace Core
