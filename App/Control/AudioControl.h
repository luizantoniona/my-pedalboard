#ifndef AUDIOCONTROL_H
#define AUDIOCONTROL_H

#include <QObject>

#include <Engine/Audio/AudioEngine.h>

class AudioControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QStringList driverAPIs READ driverAPIs NOTIFY driverAPIsChanged )
    Q_PROPERTY( QStringList inputDevices READ inputDevices NOTIFY devicesChanged )
    Q_PROPERTY( QStringList outputDevices READ outputDevices NOTIFY devicesChanged )
    Q_PROPERTY( QStringList sampleRates READ sampleRates NOTIFY sampleRatesChanged )
    Q_PROPERTY( QStringList frameBuffers READ frameBuffers NOTIFY frameBuffersChanged )

public:
    AudioControl();
    ~AudioControl();

    QStringList driverAPIs() const;
    QStringList inputDevices() const;
    QStringList outputDevices() const;
    QStringList sampleRates() const;
    QStringList frameBuffers() const;

public slots:
    void setDriverAPI( int index );
    void setInputDevice( int index );
    void setOutputDevice( int index );
    void setSampleRate( int index );
    void setFrameBuffer( int index );

    void start();
    void stop();

signals:
    void driverAPIsChanged();
    void devicesChanged();
    void sampleRatesChanged();
    void frameBuffersChanged();

private:
    Engine::AudioEngine _engine;
};

#endif // AUDIOCONTROL_H
