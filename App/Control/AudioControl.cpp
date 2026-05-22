#include "AudioControl.h"

#include <QDebug>

AudioControl::AudioControl() {
    qInfo() << "AudioControl::AudioControl";

    _engine.initialize();
    connect( &_engine, &Engine::AudioEngine::devicesChanged, this, &AudioControl::devicesChanged );
    // TODO
    // connect( &_engine, &Engine::AudioEngine::driverAPIsChanged, this, &AudioControl::driverAPIsChanged );
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

void AudioControl::setDriverAPI( int index ) {
    // TODO
}

void AudioControl::setInputDevice( int index ) {
    _engine.setInputDevice( index );
}

void AudioControl::setOutputDevice( int index ) {
    _engine.setOutputDevice( index );
}

void AudioControl::start() {
    _engine.start();
}

void AudioControl::stop() {
    _engine.stop();
}
