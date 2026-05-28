#ifndef AUDIOCHAIN_H
#define AUDIOCHAIN_H

#include <memory>
#include <vector>

#include "DSPModule.h"

namespace Engine {

class AudioChain {
public:
    void addModule( std::shared_ptr<DSPModule> modules );

    void process( float* left, float* right, unsigned int frames );

private:
    std::vector<std::shared_ptr<DSPModule>> _modules;
};

} // namespace Engine

#endif
