#include "AudioControl.h"

#include <QDebug>

AudioControl::AudioControl() {
    qInfo() << "AudioControl::AudioControl";

    _engine.initialize();
    connect( &_engine, &Engine::AudioEngine::devicesChanged, this, &AudioControl::devicesChanged );
}

AudioControl::~AudioControl() {
    qInfo() << "AudioControl::~AudioControl";
}

QStringList AudioControl::inputDevices() const {
    return _engine.inputDevices();
}

QStringList AudioControl::outputDevices() const {
    return _engine.outputDevices();
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
