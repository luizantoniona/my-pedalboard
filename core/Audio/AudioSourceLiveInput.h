#pragma once

#include "AudioSource.h"

class AudioSourceLiveInput : public AudioSource {
public:
    AudioSourceLiveInput();
    ~AudioSourceLiveInput();

    void start() override;

    void stop() override;

    float* getBuffer() override;

private:
};
