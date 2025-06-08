#include "AudioDeviceManager.h"

AudioDeviceManager::AudioDeviceManager( QObject* parent ) :
    QObject( parent ),
    _devices( new QMediaDevices( this ) ) {

    connect( _devices, &QMediaDevices::audioInputsChanged, this, &AudioDeviceManager::inputDevicesChanged );
    connect( _devices, &QMediaDevices::audioOutputsChanged, this, &AudioDeviceManager::outputDevicesChanged );
}

QList<QAudioDevice> AudioDeviceManager::inputDevices() const {
    return _devices->audioInputs();
}

QList<QAudioDevice> AudioDeviceManager::outputDevices() const {
    return _devices->audioOutputs();
}
