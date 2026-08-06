#include "AudioDevice.h"

#include <QDebug>

namespace Engine {

AudioDevice::AudioDevice() = default;

AudioDevice::~AudioDevice() {
    close();
}

QStringList AudioDevice::enumerateInputs( QVector<unsigned int>& outIds ) {
    QStringList list;
    outIds.clear();
    for ( auto id : _audio.getDeviceIds() ) {
        auto info = _audio.getDeviceInfo( id );
        if ( info.inputChannels > 0 ) {
            outIds.push_back( id );
            list << QString::fromStdString( info.name );
        }
    }
    return list;
}

QStringList AudioDevice::enumerateOutputs( QVector<unsigned int>& outIds ) {
    QStringList list;
    outIds.clear();
    for ( auto id : _audio.getDeviceIds() ) {
        auto info = _audio.getDeviceInfo( id );
        if ( info.outputChannels > 0 ) {
            outIds.push_back( id );
            list << QString::fromStdString( info.name );
        }
    }
    return list;
}

void AudioDevice::setCallback( ProcessCallback callback ) {
    _callback = std::move( callback );
}

void AudioDevice::open( unsigned int inputId, unsigned int outputId,
                        unsigned int sampleRate, unsigned int bufferSize ) {
    close();

    RtAudio::StreamParameters inParams, outParams;
    if ( inputId != 0 ) {
        inParams.deviceId = inputId;
        inParams.nChannels = 1;
    }
    if ( outputId != 0 ) {
        outParams.deviceId = outputId;
        outParams.nChannels = 2;
    }

    try {
        _audio.openStream( outputId ? &outParams : nullptr,
                           inputId ? &inParams : nullptr,
                           RTAUDIO_FLOAT32, sampleRate, &bufferSize,
                           &AudioDevice::rtCallback, this );
        _audio.startStream();
        qInfo() << "AudioDevice: stream opened" << sampleRate << "Hz," << bufferSize << "frames";
    } catch ( ... ) {
        qWarning() << "AudioDevice: failed to open stream";
    }
}

void AudioDevice::close() {
    if ( _audio.isStreamOpen() ) {
        _audio.closeStream();
        qInfo() << "AudioDevice: stream closed";
    }
}

bool AudioDevice::isOpen() const {
    return _audio.isStreamOpen();
}

int AudioDevice::rtCallback( void* out, void* in, unsigned int nFrames,
                             double, RtAudioStreamStatus, void* userData ) {
    auto* self = static_cast<AudioDevice*>( userData );
    if ( self->_callback ) {
        self->_callback( static_cast<const float*>( in ),
                         static_cast<float*>( out ),
                         static_cast<uint32_t>( nFrames ) );
    }
    return 0;
}

} // namespace Engine
