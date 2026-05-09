#ifndef AUDIOEFFECT_H
#define AUDIOEFFECT_H

namespace Engine {

class AudioEffect {
public:
    virtual ~AudioEffect() = default;

    virtual void process( float* left, float* right, unsigned int frames ) = 0;
};

} // namespace Engine

#endif
