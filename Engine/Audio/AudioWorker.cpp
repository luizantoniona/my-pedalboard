#include "AudioWorker.h"

#include <QDebug>

namespace Engine {

AudioWorker::AudioWorker( QObject* parent ) :
    QObject( parent ),
    _audio(),
    _inputIds(),
    _outputIds(),
    _inputId( 0 ),
    _outputId( 0 ),
    _sampleRate( 48000 ),
    _frameBuffer( 256 ),
    _isRunning( false ) {

    qInfo() << "AudioWorker::AudioWorker";
}

AudioWorker::~AudioWorker() {
    qInfo() << "AudioWorker::~AudioWorker";
}

void AudioWorker::start() {
    if ( _isRunning ) {
        return;
    }

    if ( _inputId == 0 && _outputId == 0 ) {
        emit error( "No device selected" );
        return;
    }

    openStream();

    qInfo() << "AudioWorker::start - started";
}

void AudioWorker::stop() {
    if ( !_isRunning ) {
        return;
    }

    closeStream();

    qInfo() << "AudioWorker::stop - stopped";
}

void AudioWorker::requestDevices() {
    auto inputs = enumerateInputs();
    auto outputs = enumerateOutputs();

    emit devicesReady( inputs, outputs );
}

void AudioWorker::setInputDevice( int index ) {
    if ( index < 0 || index >= _inputIds.size() ) {
        return;
    }
    _inputId = _inputIds[ index ];

    if ( _isRunning ) {
        qInfo() << "AudioWorker::setInputDevice - restarting stream with new input device";
        stop();
        start();
    }
}

void AudioWorker::setOutputDevice( int index ) {
    if ( index < 0 || index >= _outputIds.size() ) {
        return;
    }
    _outputId = _outputIds[ index ];

    if ( _isRunning ) {
        qInfo() << "AudioWorker::setOutputDevice - restarting stream with new output device";
        stop();
        start();
    }
}

void AudioWorker::setSampleRate( unsigned int sampleRate ) {
    if ( sampleRate == _sampleRate ) {
        return;
    }

    _sampleRate = sampleRate;

    if ( _isRunning ) {
        restartStream();
    }
}

unsigned int AudioWorker::sampleRate() const {
    return _sampleRate;
}

void AudioWorker::setFrameBuffer( unsigned int frameBuffer ) {
    if ( frameBuffer == _frameBuffer ) {
        return;
    }

    _frameBuffer = frameBuffer;

    if ( _isRunning ) {
        restartStream();
    }
}

unsigned int AudioWorker::frameBuffer() const {
    return _frameBuffer;
}

QStringList AudioWorker::enumerateInputs() {
    QStringList list;
    _inputIds.clear();

    auto ids = _audio.getDeviceIds();

    for ( auto id : ids ) {
        auto info = _audio.getDeviceInfo( id );
        if ( info.inputChannels > 0 ) {
            _inputIds.push_back( id );
            list << QString::fromStdString( info.name );
        }
    }

    return list;
}

QStringList AudioWorker::enumerateOutputs() {
    QStringList list;
    _outputIds.clear();

    auto ids = _audio.getDeviceIds();

    for ( auto id : ids ) {
        auto info = _audio.getDeviceInfo( id );
        if ( info.outputChannels > 0 ) {
            _outputIds.push_back( id );
            list << QString::fromStdString( info.name );
        }
    }

    return list;
}

void AudioWorker::openStream() {
    closeStream();

    RtAudio::StreamParameters inParams, outParams;
    unsigned int inputChannels = 0;
    unsigned int outputChannels = 0;

    if ( _inputId != 0 ) {
        inParams.deviceId = _inputId;
        inParams.nChannels = 1;
        inputChannels = 1;
    }

    if ( _outputId != 0 ) {
        outParams.deviceId = _outputId;
        outParams.nChannels = 2;
        outputChannels = 2;
    }

    try {
        _audio.openStream( _outputId ? &outParams : nullptr, _inputId ? &inParams : nullptr, RTAUDIO_FLOAT32, _sampleRate, &_frameBuffer, &AudioWorker::callback, this );
        _audio.startStream();

        _isRunning = true;

        qInfo() << "AudioWorker::openStream - started with"
                << inputChannels << "input channels,"
                << outputChannels << "output channels,"
                << _frameBuffer << "frame buffer";

    } catch ( ... ) {
        emit error( "Failed to open stream" );
    }
}

void AudioWorker::closeStream() {
    if ( _audio.isStreamOpen() ) {
        _audio.closeStream();
        _isRunning = false;
        qInfo() << "AudioWorker::closeStream - stream closed";
    }
}

void AudioWorker::restartStream() {
    qInfo() << "AudioWorker::restartStream";
    stop();
    start();
}

int AudioWorker::callback( void* out, void* in, unsigned int nFrames, double, RtAudioStreamStatus status, void* userData ) {
    auto* self = static_cast<AudioWorker*>( userData );

    self->process( static_cast<const float*>( in ), static_cast<float*>( out ), nFrames );

    return 0;
}

void AudioWorker::process( const float* in, float* out, unsigned int nFrames ) {
    for ( unsigned int i = 0; i < nFrames; i++ ) {
        float s = in ? in[ i ] : 0.0f;

        out[ 2 * i ] = s * 5;
        out[ 2 * i + 1 ] = s * 5;
    }
}

} // namespace Engine
