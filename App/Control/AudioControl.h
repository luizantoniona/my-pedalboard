#ifndef AUDIOCONTROL_H
#define AUDIOCONTROL_H

#include <QObject>

#include <Engine/Audio/AudioEngine.h>

class AudioControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QStringList driverAPIs READ driverAPIs NOTIFY driverAPIsChanged )
    Q_PROPERTY( QStringList inputDevices READ inputDevices NOTIFY devicesChanged )
    Q_PROPERTY( QStringList outputDevices READ outputDevices NOTIFY devicesChanged )

public:
    AudioControl();
    ~AudioControl();

    QStringList driverAPIs() const;
    QStringList inputDevices() const;
    QStringList outputDevices() const;

public slots:
    void setDriverAPI( int index );
    void setInputDevice( int index );
    void setOutputDevice( int index );

    void start();
    void stop();

signals:
    void driverAPIsChanged();
    void devicesChanged();

private:
    Engine::AudioEngine _engine;
};

#endif // AUDIOCONTROL_H
