#include <App/Application/Settings/SettingsPageControl.h>

#include <Engine/Audio/AudioEngine.h>

SettingsPageControl::SettingsPageControl( Engine::AudioEngine& engine, QObject* parent ) :
    QObject( parent ),
    _engine( engine ) {

    connect( &_engine, &Engine::AudioEngine::driverApisChanged, this, &SettingsPageControl::driverApisChanged );
    connect( &_engine, &Engine::AudioEngine::devicesChanged, this, &SettingsPageControl::devicesChanged );
    connect( &_engine, &Engine::AudioEngine::sampleRatesChanged, this, &SettingsPageControl::sampleRatesChanged );
}

QStringList SettingsPageControl::driverApis() const {
    return _engine.driverApis();
}

QStringList SettingsPageControl::inputDevices() const {
    return _engine.inputDevices();
}

QStringList SettingsPageControl::outputDevices() const {
    return _engine.outputDevices();
}

QStringList SettingsPageControl::sampleRates() const {
    return _engine.sampleRates();
}

QStringList SettingsPageControl::frameBuffers() const {
    return _engine.frameBuffers();
}

QString SettingsPageControl::currentDriverApi() const {
    return _engine.currentDriverApi();
}

void SettingsPageControl::setDriverApi( int index ) {
    _engine.setDriverApi( index );
}

void SettingsPageControl::setInputDevice( int index ) {
    _engine.setInputDevice( index );
}

void SettingsPageControl::setOutputDevice( int index ) {
    _engine.setOutputDevice( index );
}

void SettingsPageControl::setSampleRate( int index ) {
    _engine.setSampleRate( index );
}

void SettingsPageControl::setFrameBuffer( int index ) {
    _engine.setFrameBuffer( index );
}
