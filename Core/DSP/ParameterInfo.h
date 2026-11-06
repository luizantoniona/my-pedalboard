#pragma once

#include <string_view>

namespace Core {

class ParameterInfo {

public:
    std::string_view name;
    std::string_view label;
    std::string_view unit;
    float minimum;
    float maximum;
    float defaultValue;
};

} // namespace Core
