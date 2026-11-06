#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <Core/DSP/DSPModule.h>
#include <Core/Project/PedalDefinition.h>

namespace Core {

class NodeRegistry;

class PedalRegistry {

public:
    explicit PedalRegistry( const NodeRegistry& nodes );

    void registerPedal( PedalDefinition definition );

    const PedalDefinition* definition( const std::string& id ) const;
    std::vector<std::string> ids() const;

    std::unique_ptr<DSPModule> create( const std::string& id ) const;

    static PedalRegistry& withBuiltinPedals();

private:
    const NodeRegistry& _nodes;
    std::vector<std::string> _order;
    std::unordered_map<std::string, PedalDefinition> _definitions;
};

} // namespace Core
