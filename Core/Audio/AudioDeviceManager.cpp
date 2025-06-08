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

QList<QString> AudioDeviceManager::inputDevicesNames() const {
    QList<QString> inputNames = {};
    const QList<QAudioDevice> devices = inputDevices();

    for ( const QAudioDevice& device : devices ) {
        inputNames.append( device.description() );
    }

    return inputNames;
}

QList<QAudioDevice> AudioDeviceManager::outputDevices() const {
    return _devices->audioOutputs();
}

QList<QString> AudioDeviceManager::outputDevicesNames() const {
    QList<QString> outputNames = {};
    const QList<QAudioDevice> devices = outputDevices();

    for ( const QAudioDevice& device : devices ) {
        outputNames.append( device.description() );
    }

    return outputNames;
}
