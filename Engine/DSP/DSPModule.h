#ifndef DSPMODULE_H
#define DSPMODULE_H

namespace Engine {

class DSPModule {
public:
    virtual ~DSPModule() = default;

    virtual void process( float* left, float* right, unsigned int frames ) = 0;
};

} // namespace Engine

#endif // DSPMODULE_H
