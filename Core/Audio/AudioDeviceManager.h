#ifndef AUDIODEVICEMANAGER_H
#define AUDIODEVICEMANAGER_H

#include <QAudioDevice>
#include <QMediaDevices>
#include <QObject>

class AudioDeviceManager : public QObject {
    Q_OBJECT
    Q_PROPERTY( QList<QAudioDevice> inputDevices READ inputDevices NOTIFY inputDevicesChanged )
    Q_PROPERTY( QList<QAudioDevice> outputDevices READ outputDevices NOTIFY outputDevicesChanged )

public:
    explicit AudioDeviceManager( QObject* parent = nullptr );

    QList<QAudioDevice> inputDevices() const;
    QList<QAudioDevice> outputDevices() const;

signals:
    void inputDevicesChanged();
    void outputDevicesChanged();

private:
    QMediaDevices* _devices;
};

#endif // AUDIODEVICEMANAGER_H
