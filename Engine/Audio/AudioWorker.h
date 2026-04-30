#ifndef AUDIOWORKER_H
#define AUDIOWORKER_H

#include <QObject>

#include <RtAudio.h>

namespace Engine {

class AudioWorker : public QObject {
    Q_OBJECT

public:
    explicit AudioWorker( QObject* parent = nullptr );
    ~AudioWorker();

public slots:
    void start();
    void stop();

    void requestDevices();
    void setInputDevice( int index );
    void setOutputDevice( int index );

signals:
    void devicesReady( QStringList inputs, QStringList outputs );
    void started();
    void stopped();
    void error( QString message );

private:
    QStringList enumerateInputs();
    QStringList enumerateOutputs();

    void openStream();
    void closeStream();

    static int callback( void* out, void* in, unsigned int nFrames, double, RtAudioStreamStatus status, void* userData );

    void process( const float* in, float* out, unsigned int nFrames );

private:
    RtAudio _audio;

    QVector<unsigned int> _inputIds;
    QVector<unsigned int> _outputIds;

    unsigned int _inputId;
    unsigned int _outputId;

    unsigned int _sampleRate;
    unsigned int _bufferFrames;

    bool _isRunning;
};

} // namespace Engine

#endif // AUDIOWORKER_H
