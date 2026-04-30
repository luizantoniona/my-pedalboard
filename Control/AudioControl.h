#ifndef AUDIOCONTROL_H
#define AUDIOCONTROL_H

#include <QObject>

#include <Engine/Audio/AudioEngine.h>

class AudioControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QStringList inputDevices READ inputDevices NOTIFY devicesChanged )
    Q_PROPERTY( QStringList outputDevices READ outputDevices NOTIFY devicesChanged )

public:
    AudioControl();
    ~AudioControl();

    QStringList inputDevices() const;
    QStringList outputDevices() const;

    Q_INVOKABLE void setInputDevice( int index );
    Q_INVOKABLE void setOutputDevice( int index );

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

signals:
    void devicesChanged();

private:
    Engine::AudioEngine _engine;
};

#endif // AUDIOCONTROL_H
