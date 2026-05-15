#ifndef AUDIOWORKER_H
#define AUDIOWORKER_H

#include <atomic>
#include <vector>

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

    void setOutputVolume( float outputVolume );
    float outputVolume() const;

signals:
    void devicesReady( QStringList inputs, QStringList outputs );
    void error( QString message );

private:
    QStringList enumerateInputs();
    QStringList enumerateOutputs();

    void openInputStream();
    void openOutputStream();

    void closeInputStream();
    void closeOutputStream();

    void restartStreams();

    static int inputCallback( void* out, void* in, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void* userData );
    static int outputCallback( void* out, void* in, unsigned int nFrames, double streamTime, RtAudioStreamStatus status, void* userData );

    void processInput( const float* input, unsigned int nFrames );
    void processOutput( float* output, unsigned int nFrames );

private:
    RtAudio _audioInput;
    RtAudio _audioOutput;

    QVector<unsigned int> _inputIds;
    QVector<unsigned int> _outputIds;

    unsigned int _inputId;
    unsigned int _outputId;

    unsigned int _sampleRate;
    unsigned int _frameBuffer;

    std::atomic<float> _outputVolume;

    std::vector<float> _ringBuffer;

    std::vector<float> _leftBuffer;
    std::vector<float> _rightBuffer;

    std::atomic<bool> _inputRunning;
    std::atomic<bool> _outputRunning;
};

} // namespace Engine

#endif // AUDIOWORKER_H
