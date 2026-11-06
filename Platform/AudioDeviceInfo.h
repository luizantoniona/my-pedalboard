#pragma once

#include <string>
#include <vector>

namespace Platform {

class AudioDeviceInfo {

public:
    std::string name;
    unsigned int id{ 0 };
    unsigned int channels{ 0 };
    std::vector<unsigned int> sampleRates;
};

} // namespace Platform
