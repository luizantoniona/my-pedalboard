#include "AudioSourceLiveInput.h"

#include <cstring>

AudioSourceLiveInput::AudioSourceLiveInput() {
    Pa_Initialize();
}

AudioSourceLiveInput::~AudioSourceLiveInput() {
    stop();
    Pa_Terminate();
}

int AudioSourceLiveInput::paCallback( const void* input, void*, unsigned long frameCount, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void* userData ) {
    auto* self = static_cast<AudioSourceLiveInput*>( userData );
    std::memcpy( self->buffer, input, sizeof( float ) * frameCount );

    return paContinue;
}

void AudioSourceLiveInput::start() {
    PaStreamParameters inputParams;
    inputParams.device = Pa_GetDefaultInputDevice();
    inputParams.channelCount = 1;
    inputParams.sampleFormat = paFloat32;
    inputParams.suggestedLatency = Pa_GetDeviceInfo( inputParams.device )->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = nullptr;

    Pa_OpenStream( &stream, &inputParams, nullptr, 44100, 512, paNoFlag, paCallback, this );
    Pa_StartStream( stream );
}

void AudioSourceLiveInput::stop() {
    if ( stream ) {
        Pa_StopStream( stream );
        Pa_CloseStream( stream );
        stream = nullptr;
    }
}

float* AudioSourceLiveInput::getBuffer() {
    return buffer;
}