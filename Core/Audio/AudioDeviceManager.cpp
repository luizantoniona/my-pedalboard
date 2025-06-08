#include "AudioDeviceManager.h"

#include <QDebug>
#include <QTimer>

AudioDeviceManager::AudioDeviceManager( QObject* parent ) :
    QObject( parent ),
    _devices( new QMediaDevices( this ) ),
    _inputDeviceNames( {} ),
    _outputDeviceNames( {} ) {

    connect( _devices, &QMediaDevices::audioInputsChanged, this, &AudioDeviceManager::updateInputDeviceNames );
    connect( _devices, &QMediaDevices::audioOutputsChanged, this, &AudioDeviceManager::updateOutputDeviceNames );

    QTimer::singleShot( 0, this, &AudioDeviceManager::updateInputDeviceNames );
    // QTimer::singleShot( 0, this, &AudioDeviceManager::updateOutputDeviceNames );
}

QList<QAudioDevice> AudioDeviceManager::inputDevices() const {
    return _devices->audioInputs();
}

QList<QString> AudioDeviceManager::inputDevicesNames() const {
    return _inputDeviceNames;
}

QList<QAudioDevice> AudioDeviceManager::outputDevices() const {
    return _devices->audioOutputs();
}

QList<QString> AudioDeviceManager::outputDevicesNames() const {
    return _outputDeviceNames;
}

void AudioDeviceManager::updateInputDeviceNames() {

    _inputDeviceNames.clear();

    const QList<QAudioDevice> devices = inputDevices();

    for ( const QAudioDevice& device : devices ) {
        _inputDeviceNames.append( device.description() );
    }

    emit inputDevicesChanged();
}

void AudioDeviceManager::updateOutputDeviceNames() {

    _outputDeviceNames.clear();

    const QList<QAudioDevice> devices = outputDevices();

    for ( const QAudioDevice& device : devices ) {
        _outputDeviceNames.append( device.description() );
    }

    emit outputDevicesChanged();
}
