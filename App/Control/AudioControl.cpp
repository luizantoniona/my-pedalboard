#include "AudioControl.h"

#include <QDebug>

AudioControl::AudioControl( QObject* parent ) :
    QObject( parent ),
    _engine() {

    qInfo() << "AudioControl::AudioControl";

    QObject::connect( &_engine, &Engine::AudioEngine::driverAPIsChanged, this, &AudioControl::driverAPIsChanged );
    QObject::connect( &_engine, &Engine::AudioEngine::devicesChanged, this, &AudioControl::devicesChanged );
    QObject::connect( &_engine, &Engine::AudioEngine::sampleRatesChanged, this, &AudioControl::sampleRatesChanged );
    QObject::connect( &_engine, &Engine::AudioEngine::frameBuffersChanged, this, &AudioControl::frameBuffersChanged );

    qInfo() << "AudioControl::AudioControl";
}

AudioControl::~AudioControl() = default;

QList<QString> AudioControl::driverAPIs() const {
    return _engine.driverAPIs();
}

QList<QString> AudioControl::inputDevices() const {
    return _engine.inputDevices();
}

QList<QString> AudioControl::outputDevices() const {
    return _engine.outputDevices();
}

QList<QString> AudioControl::sampleRates() const {
    return _engine.sampleRates();
}

QList<QString> AudioControl::frameBuffers() const {
    return _engine.frameBuffers();
}

void AudioControl::setDriverAPI( int index ) {
    _engine.setDriverAPI( index );
}

void AudioControl::setInputDevice( int index ) {
    _engine.setInputDevice( index );
}

void AudioControl::setOutputDevice( int index ) {
    _engine.setOutputDevice( index );
}

void AudioControl::setSampleRate( int index ) {
    _engine.setSampleRate( index );
}

void AudioControl::setFrameBuffer( int index ) {
    _engine.setFrameBuffer( index );
}

void AudioControl::start() {
    _engine.start();
}

void AudioControl::stop() {
    _engine.stop();
}
