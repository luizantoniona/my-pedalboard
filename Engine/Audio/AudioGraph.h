#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <Engine/DSP/AudioNode.h>

namespace Engine {

class AudioGraph {
public:
    struct Edge {
        std::string from;
        std::string to;
    };

    AudioGraph();
    ~AudioGraph();

    void addNode( const std::string& id, std::shared_ptr<AudioNode> node );
    void removeNode( const std::string& id );
    void connect( const std::string& fromId, const std::string& toId );
    void disconnect( const std::string& fromId, const std::string& toId );

    // Called exclusively on the audio thread
    void process( AudioBuffer& buffer );

    // Read-only inspection (for serialization — call outside audio thread)
    const std::unordered_map<std::string, std::shared_ptr<AudioNode>>& nodes() const {
        return _nodes;
    }
    const std::vector<Edge>& edges() const {
        return _edges;
    }

private:
    using NodeList = std::vector<std::shared_ptr<AudioNode>>;

    void rebuildExecutionOrder();
    NodeList topologicalSort() const;

    std::unordered_map<std::string, std::shared_ptr<AudioNode>> _nodes;
    std::vector<Edge> _edges;

    // Atomic swap — lock-free read on audio thread (C++20)
    std::atomic<std::shared_ptr<const NodeList>> _executionOrder;
};

} // namespace Engine
