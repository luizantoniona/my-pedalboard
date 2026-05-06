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

    void setSampleRate( unsigned int sampleRate );
    unsigned int sampleRate() const;

    void setFrameBuffer( unsigned int frameBuffer );
    unsigned int frameBuffer() const;

signals:
    void devicesReady( QStringList inputs, QStringList outputs );
    void error( QString message );

private:
    QStringList enumerateInputs();
    QStringList enumerateOutputs();

    void openStream();
    void closeStream();
    void restartStream();

    static int callback( void* out, void* in, unsigned int nFrames, double, RtAudioStreamStatus status, void* userData );

    void process( const float* in, float* out, unsigned int nFrames );

private:
    RtAudio _audio;

    QVector<unsigned int> _inputIds;
    QVector<unsigned int> _outputIds;

    unsigned int _inputId;
    unsigned int _outputId;

    unsigned int _sampleRate;
    unsigned int _frameBuffer;

    bool _isRunning;
};

} // namespace Engine

#endif // AUDIOWORKER_H
