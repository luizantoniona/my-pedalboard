#include "AudioDeviceModel.h"

namespace Models {

AudioDeviceModel::AudioDeviceModel( Engine::AudioEngine& engine, QObject* parent ) : QObject( parent ), _engine( engine ) {
    connect( &_engine, &Engine::AudioEngine::devicesChanged,
             this, &AudioDeviceModel::devicesChanged );
}

QStringList AudioDeviceModel::inputDevices() const {
    return _engine.inputDevices();
}

QStringList AudioDeviceModel::outputDevices() const {
    return _engine.outputDevices();
}

int AudioDeviceModel::currentInput() const {
    return _currentInput;
}

int AudioDeviceModel::currentOutput() const {
    return _currentOutput;
}

void AudioDeviceModel::setCurrentInput( int index ) {
    if ( index == _currentInput )
        return;
    _currentInput = index;
    _engine.setInputDevice( index );
    emit currentInputChanged();
}

void AudioDeviceModel::setCurrentOutput( int index ) {
    if ( index == _currentOutput )
        return;
    _currentOutput = index;
    _engine.setOutputDevice( index );
    emit currentOutputChanged();
}

} // namespace Models
