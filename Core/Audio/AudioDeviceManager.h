#ifndef AUDIODEVICEMANAGER_H
#define AUDIODEVICEMANAGER_H

#include <QAudioDevice>
#include <QMediaDevices>
#include <QObject>

class AudioDeviceManager : public QObject {
    Q_OBJECT
    Q_PROPERTY( QList<QString> inputDevicesNames READ inputDevicesNames NOTIFY inputDevicesChanged )
    Q_PROPERTY( QList<QString> outputDevicesNames READ outputDevicesNames NOTIFY outputDevicesChanged )

public:
    explicit AudioDeviceManager( QObject* parent = nullptr );

    QList<QAudioDevice> inputDevices() const;
    QList<QString> inputDevicesNames() const;

    QList<QAudioDevice> outputDevices() const;
    QList<QString> outputDevicesNames() const;

signals:
    void inputDevicesChanged();
    void outputDevicesChanged();

private:
    void updateInputDeviceNames();
    void updateOutputDeviceNames();

    QMediaDevices* _devices;
    QList<QString> _inputDeviceNames;
    QList<QString> _outputDeviceNames;
};

#endif // AUDIODEVICEMANAGER_H
