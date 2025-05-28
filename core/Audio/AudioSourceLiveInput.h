#pragma once

#include <portaudio.h>

#include "AudioSource.h"

class AudioSourceLiveInput : public AudioSource {
public:
    AudioSourceLiveInput();
    ~AudioSourceLiveInput();

    void start() override;

    void stop() override;

    float* getBuffer() override;

private:
    PaStream* stream = nullptr;
    float buffer[ 512 ]; // Exemplo fixo
    static int paCallback( const void*, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void* userData );
};
