#ifndef AUDIOCONTROL_H
#define AUDIOCONTROL_H

#include <QObject>

#include <Engine/Audio/AudioEngine.h>

class AudioControl : public QObject {
    Q_OBJECT
    Q_PROPERTY( QList<QString> driverAPIs READ driverAPIs NOTIFY driverAPIsChanged )
    Q_PROPERTY( QList<QString> inputDevices READ inputDevices NOTIFY devicesChanged )
    Q_PROPERTY( QList<QString> outputDevices READ outputDevices NOTIFY devicesChanged )
    Q_PROPERTY( QList<QString> sampleRates READ sampleRates NOTIFY sampleRatesChanged )
    Q_PROPERTY( QList<QString> frameBuffers READ frameBuffers NOTIFY frameBuffersChanged )

public:
    explicit AudioControl( QObject* parent = nullptr );
    ~AudioControl();

    QList<QString> driverAPIs() const;
    QList<QString> inputDevices() const;
    QList<QString> outputDevices() const;
    QList<QString> sampleRates() const;
    QList<QString> frameBuffers() const;

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
