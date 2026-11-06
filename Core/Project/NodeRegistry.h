#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <Core/DSP/DSPModule.h>

namespace Core {

class NodeRegistry {

public:
    using Creator = std::function<std::unique_ptr<DSPModule>()>;

    void registerType( std::string typeName, Creator creator );
    std::unique_ptr<DSPModule> create( const std::string& typeName ) const;
    bool hasType( const std::string& typeName ) const;
    std::vector<std::string> typeNames() const;

    static NodeRegistry& withBuiltinTypes();

private:
    std::unordered_map<std::string, Creator> _creators;
};

} // namespace Core
