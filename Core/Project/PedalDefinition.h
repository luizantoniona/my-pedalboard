#pragma once

#include <string>
#include <vector>

#include <Core/Project/ParameterOverride.h>

namespace Core {

class PedalDefinition {

public:
    std::string id;
    std::string displayName;
    std::string algorithm;
    std::vector<ParameterOverride> overrides;
};

} // namespace Core
