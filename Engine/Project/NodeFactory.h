#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include <Engine/DSP/DSPModule.h>

namespace Engine {

// Maps type name strings (e.g. "Gain") to DSPModule factory functions.
// Register all effect types before loading presets or building chains.
class NodeFactory {
public:
    using Creator = std::function<std::unique_ptr<DSPModule>()>;

    void registerType( std::string typeName, Creator creator );
    std::unique_ptr<DSPModule> create( const std::string& typeName ) const;
    bool hasType( const std::string& typeName ) const;

    const std::unordered_map<std::string, Creator>& types() const {
        return _creators;
    }

private:
    std::unordered_map<std::string, Creator> _creators;
};

} // namespace Engine
