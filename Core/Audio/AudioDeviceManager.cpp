#include "AudioDeviceManager.h"

#include <QTimer>

AudioDeviceManager::AudioDeviceManager( QObject* parent ) :
    QObject( parent ),
    _mediaDevices(),
    _inputDeviceNames( {} ),
    _outputDeviceNames( {} ) {

    connect( &_mediaDevices, &QMediaDevices::audioInputsChanged, this, &AudioDeviceManager::updateInputDeviceNames );
    connect( &_mediaDevices, &QMediaDevices::audioOutputsChanged, this, &AudioDeviceManager::updateOutputDeviceNames );

    QTimer::singleShot( 0, this, &AudioDeviceManager::updateInputDeviceNames );
    QTimer::singleShot( 0, this, &AudioDeviceManager::updateOutputDeviceNames );
}

QList<QAudioDevice> AudioDeviceManager::inputDevices() const {
    return _mediaDevices.audioInputs();
}

QList<QString> AudioDeviceManager::inputDevicesNames() const {
    return _inputDeviceNames;
}

QList<QAudioDevice> AudioDeviceManager::outputDevices() const {
    return _mediaDevices.audioOutputs();
}

QList<QString> AudioDeviceManager::outputDevicesNames() const {
    return _outputDeviceNames;
}

QAudioDevice AudioDeviceManager::inputDeviceByName( const QString& name ) {
    const auto devices = inputDevices();

    for ( const QAudioDevice& device : devices ) {

        if ( device.description() == name ) {
            return device;
        }
    }

    return _mediaDevices.defaultAudioInput();
}

QAudioDevice AudioDeviceManager::outputDeviceByName( const QString& name ) {
    const auto devices = outputDevices();

    for ( const QAudioDevice& device : devices ) {

        if ( device.description() == name ) {
            return device;
        }
    }

    return _mediaDevices.defaultAudioOutput();
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
