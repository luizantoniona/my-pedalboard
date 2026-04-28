#include "AudioEngine.h"

#include <QDebug>

AudioEngine::AudioEngine( QObject* parent ) : QObject( parent ) {
}

AudioEngine::~AudioEngine() {
    closeStream();
}

QStringList AudioEngine::inputDevices() {
    QStringList list;
    _inputDeviceIds.clear();

    auto deviceIds = _audio.getDeviceIds();

    for ( auto id : deviceIds ) {
        try {
            auto info = _audio.getDeviceInfo( id );

            if ( info.inputChannels > 0 ) {
                _inputDeviceIds.push_back( id );
                list << QString::fromStdString( info.name );
            }
        } catch ( const std::exception& e ) {
            qDebug() << "Erro ao acessar device:" << id << "-" << e.what();
        }
    }

    return list;
}

QStringList AudioEngine::outputDevices() {
    QStringList list;
    _outputDeviceIds.clear();

    auto deviceIds = _audio.getDeviceIds();

    for ( auto id : deviceIds ) {
        try {
            auto info = _audio.getDeviceInfo( id );

            if ( info.outputChannels > 0 ) {
                _outputDeviceIds.push_back( id );
                list << QString::fromStdString( info.name );
            }
        } catch ( const std::exception& e ) {
            qDebug() << "Erro ao acessar device:" << id << "-" << e.what();
        }
    }

    return list;
}

void AudioEngine::setInputDevice( const QString& name ) {
    for ( int idx = 0; idx < _inputDeviceIds.size(); idx++ ) {
        try {
            auto info = _audio.getDeviceInfo( _inputDeviceIds[ idx ] );

            if ( QString::fromStdString( info.name ) == name ) {
                _inputDevice = _inputDeviceIds[ idx ];
                openStream();
                return;
            }
        } catch ( ... ) {
        }
    }

    qWarning() << "Input device não encontrado:" << name;
}

void AudioEngine::setOutputDevice( const QString& name ) {
    for ( int idx = 0; idx < _outputDeviceIds.size(); idx++ ) {
        try {
            auto info = _audio.getDeviceInfo( _outputDeviceIds[ idx ] );

            if ( QString::fromStdString( info.name ) == name ) {
                _outputDevice = _outputDeviceIds[ idx ];
                openStream();
                return;
            }
        } catch ( ... ) {
        }
    }

    qWarning() << "Output device não encontrado:" << name;
}

void AudioEngine::setBypass( bool enabled ) {
    _bypass = enabled;
}

void AudioEngine::openStream() {
    closeStream();

    if ( _inputDevice == 0 && _outputDevice == 0 ) {
        qWarning() << "Nenhum device selecionado";
        return;
    }

    RtAudio::StreamParameters inputParams, outputParams;

    inputParams.deviceId = _inputDevice;
    inputParams.nChannels = 1;

    outputParams.deviceId = _outputDevice;
    outputParams.nChannels = 2;

    try {
        _audio.openStream( &outputParams, &inputParams, RTAUDIO_FLOAT32, _sampleRate, &_bufferFrames, &AudioEngine::audioCallback, this );
        _audio.startStream();

        qDebug() << "Stream iniciado com sucesso";

    } catch ( ... ) {
        qWarning() << "RtAudio error:";
    }
}

void AudioEngine::closeStream() {
    if ( _audio.isStreamOpen() ) {
        try {
            _audio.stopStream();
        } catch ( ... ) {
        }

        _audio.closeStream();
    }
}

int AudioEngine::audioCallback( void* outputBuffer, void* inputBuffer, unsigned int nFrames, double, RtAudioStreamStatus status, void* userData ) {
    auto* engine = static_cast<AudioEngine*>( userData );

    if ( status == RTAUDIO_INPUT_OVERFLOW ) {
        qInfo() << "Overflow detected";
    }

    if ( status == RTAUDIO_OUTPUT_UNDERFLOW ) {
        qInfo() << "Underflow detected";
    }

    float* in = static_cast<float*>( inputBuffer );
    float* out = static_cast<float*>( outputBuffer );

    if ( !in || !out )
        return 0;

    for ( unsigned int i = 0; i < nFrames; i++ ) {
        float sample = engine->m_bypass ? in[ i ] : 0.0f;

        out[ i * 2 ] = sample;
        out[ i * 2 + 1 ] = sample;
    }

    return 0;
}
