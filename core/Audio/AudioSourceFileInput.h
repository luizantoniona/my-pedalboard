#pragma once

#include <QString>

#include "AudioSource.h"

class AudioSourceFileInput : public AudioSource {
public:
    AudioSourceFileInput();
    ~AudioSourceFileInput();

    void start() override;

    void stop() override;

    float* getBuffer() override;

private:
};
