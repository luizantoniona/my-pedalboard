#include "AudioControl.h"

#include <QDebug>

AudioControl::AudioControl() {
    qInfo() << "AudioControl::AudioControl";

    _engine.initialize();
    connect( &_engine, &Engine::AudioEngine::devicesChanged, this, &AudioControl::devicesChanged );
    // TODO
    // connect( &_engine, &Engine::AudioEngine::driverAPIsChanged, this, &AudioControl::driverAPIsChanged );
    // connect( &_engine, &Engine::AudioEngine::sampleRatesChanged, this, &AudioControl::sampleRatesChanged );
    // connect( &_engine, &Engine::AudioEngine::frameBuffersChanged, this, &AudioControl::frameBuffersChanged );
}

AudioControl::~AudioControl() {
    qInfo() << "AudioControl::~AudioControl";
}

QStringList AudioControl::driverAPIs() const {
    // TODO
    return {};
}

QStringList AudioControl::inputDevices() const {
    return _engine.inputDevices();
}

QStringList AudioControl::outputDevices() const {
    return _engine.outputDevices();
}

QStringList AudioControl::sampleRates() const {
    // TODO
    return {};
}

QStringList AudioControl::frameBuffers() const {
    // TODO
    return {};
}

void AudioControl::setDriverAPI( int index ) {
    // TODO
}

void AudioControl::setInputDevice( int index ) {
    _engine.setInputDevice( index );
}

void AudioControl::setOutputDevice( int index ) {
    _engine.setOutputDevice( index );
}

void AudioControl::setSampleRate( int index ) {
    // TODO
}

void AudioControl::setFrameBuffer( int index ) {
    // TODO
}

void AudioControl::start() {
    _engine.start();
}

void AudioControl::stop() {
    _engine.stop();
}
