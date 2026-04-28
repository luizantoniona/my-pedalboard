#pragma once

#include <QObject>
#include <QStringList>

#include <RtAudio.h>

class AudioEngine : public QObject {
    Q_OBJECT
    Q_PROPERTY( QStringList inputDevices READ inputDevices NOTIFY devicesChanged )
    Q_PROPERTY( QStringList outputDevices READ outputDevices NOTIFY devicesChanged )

public:
    explicit AudioEngine( QObject* parent = nullptr );
    ~AudioEngine();

    QStringList inputDevices();
    QStringList outputDevices();

    Q_INVOKABLE void setInputDevice( const QString& name );
    Q_INVOKABLE void setOutputDevice( const QString& name );
    Q_INVOKABLE void setBypass( bool enabled );

signals:
    void devicesChanged();

private:
    void openStream();
    void closeStream();

    static int audioCallback( void* outputBuffer, void* inputBuffer, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void* userData );

private:
    RtAudio _audio;
    QVector<unsigned int> _inputDeviceIds;
    QVector<unsigned int> _outputDeviceIds;
    unsigned int _inputDevice = 0;
    unsigned int _outputDevice = 0;
    unsigned int _sampleRate = 48000;
    unsigned int _bufferFrames = 256;

    bool m_bypass = true;
};
