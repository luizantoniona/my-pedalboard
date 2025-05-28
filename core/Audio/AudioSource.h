#pragma once

class AudioSource {
public:
    virtual ~AudioSource() = default;

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual float* getBuffer() = 0;
};
