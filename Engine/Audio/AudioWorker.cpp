#include "AudioWorker.h"

#include <QDebug>

namespace Engine {

AudioWorker::AudioWorker( QObject* parent ) :
    QObject( parent ),
    _inputId( std::numeric_limits<unsigned int>::max() ),
    _outputId( std::numeric_limits<unsigned int>::max() ),
    _sampleRate( 96000 ),
    _frameBuffer( 128 ),
    _outputVolume( 1.0f ),
    _inputRunning( false ),
    _outputRunning( false ) {

    qInfo() << "AudioWorker";

    _leftBuffer.resize( _frameBuffer );
    _rightBuffer.resize( _frameBuffer );

    requestDevices();

    qInfo() << "AudioWorker";
}

AudioWorker::~AudioWorker() {
    qInfo() << "~AudioWorker";

    stop();

    qInfo() << "~AudioWorker";
}

void AudioWorker::start() {
    openInputStream();
    openOutputStream();
}

void AudioWorker::stop() {
    closeInputStream();
    closeOutputStream();
}

void AudioWorker::requestDevices() {
    emit devicesReady( enumerateInputs(), enumerateOutputs() );
}

void AudioWorker::setInputDevice( int index ) {
    if ( index < 0 || index >= _inputIds.size() ) {
        return;
    }

    _inputId = _inputIds[ index ];

    qInfo() << "Input device changed:" << index;

    if ( _inputRunning ) {
        closeInputStream();
        openInputStream();
    }
}

void AudioWorker::setOutputDevice( int index ) {
    if ( index < 0 || index >= _outputIds.size() ) {
        return;
    }

    _outputId = _outputIds[ index ];

    qInfo() << "Output device changed:" << index;

    if ( _outputRunning ) {
        closeOutputStream();
        openOutputStream();
    }
}

void AudioWorker::setSampleRate( unsigned int sampleRate ) {
    if ( sampleRate == _sampleRate ) {
        return;
    }

    _sampleRate = sampleRate;

    restartStreams();
}

unsigned int AudioWorker::sampleRate() const {
    return _sampleRate;
}

void AudioWorker::setFrameBuffer( unsigned int frameBuffer ) {
    if ( frameBuffer == _frameBuffer ) {
        return;
    }

    _frameBuffer = frameBuffer;

    _leftBuffer.resize( _frameBuffer );
    _rightBuffer.resize( _frameBuffer );

    restartStreams();
}

unsigned int AudioWorker::frameBuffer() const {
    return _frameBuffer;
}

void AudioWorker::setOutputVolume( float outputVolume ) {
    _outputVolume.store( std::clamp( outputVolume, 0.0f, 4.0f ) );
}

float AudioWorker::outputVolume() const {
    return _outputVolume.load();
}

QStringList AudioWorker::enumerateInputs() {
    QStringList list;

    _inputIds.clear();

    auto ids = _audioInput.getDeviceIds();

    for ( auto id : ids ) {

        auto info = _audioInput.getDeviceInfo( id );

        if ( info.inputChannels <= 0 ) {
            continue;
        }

        _inputIds.push_back( id );

        list << QString::fromStdString( info.name );

        qInfo() << "[INPUT]" << QString::fromStdString( info.name ) << "id:" << id;
    }

    if ( !_inputIds.isEmpty() ) {
        _inputId = _inputIds.first();
    }

    return list;
}

QStringList AudioWorker::enumerateOutputs() {
    QStringList list;

    _outputIds.clear();

    auto ids = _audioOutput.getDeviceIds();

    for ( auto id : ids ) {

        auto info = _audioOutput.getDeviceInfo( id );

        if ( info.outputChannels <= 0 ) {
            continue;
        }

        _outputIds.push_back( id );

        list << QString::fromStdString( info.name );

        qInfo() << "[OUTPUT]" << QString::fromStdString( info.name ) << "id:" << id;
    }

    if ( !_outputIds.isEmpty() ) {
        _outputId = _outputIds.first();
    }

    return list;
}

void AudioWorker::openInputStream() {
    if ( _inputId == std::numeric_limits<unsigned int>::max() ) {
        return;
    }

    closeInputStream();

    try {
        auto info = _audioInput.getDeviceInfo( _inputId );

        RtAudio::StreamParameters params;
        params.deviceId = _inputId;
        params.nChannels = std::min( 2u, info.inputChannels );

        _audioInput.openStream( nullptr, &params, RTAUDIO_FLOAT32, _sampleRate, &_frameBuffer, &AudioWorker::inputCallback, this );
        _audioInput.startStream();

        _inputRunning = true;

        qInfo() << "Input stream started";

    } catch ( RtAudioErrorType& e ) {
        qWarning() << "[INPUT ERROR]" << e;
    }
}

void AudioWorker::openOutputStream() {
    if ( _outputId == std::numeric_limits<unsigned int>::max() ) {
        return;
    }

    closeOutputStream();

    try {
        RtAudio::StreamParameters params;
        params.deviceId = _outputId;
        params.nChannels = 2;

        _audioOutput.openStream( &params, nullptr, RTAUDIO_FLOAT32, _sampleRate, &_frameBuffer, &AudioWorker::outputCallback, this );
        _audioOutput.startStream();

        _outputRunning = true;

        qInfo() << "Output stream started";

    } catch ( RtAudioErrorType& e ) {
        qWarning() << "[OUTPUT ERROR]" << e;
    }
}

void AudioWorker::closeInputStream() {
    try {
        if ( _audioInput.isStreamRunning() ) {
            _audioInput.stopStream();
        }

        if ( _audioInput.isStreamOpen() ) {
            _audioInput.closeStream();
        }

    } catch ( RtAudioErrorType& e ) {
        qWarning() << "[INPUT ERROR]" << e;
    }

    _inputRunning = false;
}

void AudioWorker::closeOutputStream() {
    try {
        if ( _audioOutput.isStreamRunning() ) {
            _audioOutput.stopStream();
        }

        if ( _audioOutput.isStreamOpen() ) {
            _audioOutput.closeStream();
        }

    } catch ( RtAudioErrorType& e ) {
        qWarning() << "[OUTPUT ERROR]" << e;
    }

    _outputRunning = false;
}

void AudioWorker::restartStreams() {
    stop();
    start();
}

int AudioWorker::inputCallback( void*, void* in, unsigned int nFrames, double, RtAudioStreamStatus, void* userData ) {
    auto* self = static_cast<AudioWorker*>( userData );
    self->processInput( static_cast<const float*>( in ), nFrames );
    return 0;
}

int AudioWorker::outputCallback( void* out, void*, unsigned int nFrames, double, RtAudioStreamStatus, void* userData ) {
    auto* self = static_cast<AudioWorker*>( userData );
    self->processOutput( static_cast<float*>( out ), nFrames );
    return 0;
}

void AudioWorker::processInput( const float* input, unsigned int nFrames ) {
    if ( !input ) {
        return;
    }

    for ( unsigned int i = 0; i < nFrames; ++i ) {
        _ringBuffer.push_back( input[ i ] );
    }
}

void AudioWorker::processOutput( float* output, unsigned int nFrames ) {
    if ( !output ) {
        return;
    }

    float volume = _outputVolume.load();

    for ( unsigned int i = 0; i < nFrames; ++i ) {

        float sample = 0.0f;

        if ( !_ringBuffer.empty() ) {
            sample = _ringBuffer.front();
            _ringBuffer.erase( _ringBuffer.begin() );
        }

        sample *= volume;
        _leftBuffer[ i ] = std::clamp( sample, -1.0f, 1.0f );
        _rightBuffer[ i ] = std::clamp( sample, -1.0f, 1.0f );

        output[ 2 * i ] = _leftBuffer[ i ];
        output[ 2 * i + 1 ] = _rightBuffer[ i ];
    }
}

} // namespace Engine
