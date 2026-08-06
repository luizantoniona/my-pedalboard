#pragma once

#include <QJsonObject>

#include "NodeFactory.h"
#include <Engine/Audio/AudioGraph.h>

namespace Engine {

// Serializes/deserializes an AudioGraph to/from JSON.
class Serializer {
public:
    explicit Serializer( NodeFactory& factory );

    // Returns false and leaves graph untouched on error
    bool load( const QJsonObject& root, AudioGraph& graph ) const;
    QJsonObject save( AudioGraph& graph ) const;

private:
    NodeFactory& _factory;
};

} // namespace Engine
