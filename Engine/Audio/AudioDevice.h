#pragma once

#include <cstdint>
#include <functional>

#include <QStringList>
#include <QVector>
#include <RtAudio.h>

namespace Engine {

class AudioDevice {
public:
    using ProcessCallback = std::function<void ( const float* in, float* out, uint32_t frames )>;

    AudioDevice();
    ~AudioDevice();

    QStringList enumerateInputs( QVector<unsigned int>& outIds );
    QStringList enumerateOutputs( QVector<unsigned int>& outIds );

    void setCallback( ProcessCallback callback );
    void open( unsigned int inputId, unsigned int outputId,
               unsigned int sampleRate, unsigned int bufferSize );
    void close();
    bool isOpen() const;

private:
    static int rtCallback( void* out, void* in, unsigned int nFrames,
                           double, RtAudioStreamStatus, void* userData );

    RtAudio _audio;
    ProcessCallback _callback;
};

} // namespace Engine
