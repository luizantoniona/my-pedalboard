#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QThread>

#include "AudioWorker.h"

namespace Engine {

class AudioEngine : public QObject {
    Q_OBJECT

public:
    explicit AudioEngine( QObject* parent = nullptr );
    ~AudioEngine();

    QList<QString> driverAPIs() const;
    QList<QString> inputDevices() const;
    QList<QString> outputDevices() const;
    QList<QString> sampleRates() const;
    QList<QString> frameBuffers() const;

    void setDriverAPI( int index );
    void setInputDevice( int index );
    void setOutputDevice( int index );
    void setSampleRate( int index );
    void setFrameBuffer( int index );

    void initialize();
    void deinitialize();

    void start();
    void stop();

signals:
    void driverAPIsChanged();
    void devicesChanged();
    void sampleRatesChanged();
    void frameBuffersChanged();

private:
    AudioWorker* _worker;

    QThread _workerThread;

    RtAudio::Api _driverAPI;

    std::vector<RtAudio::Api> _compiledAPIs;

    QList<QString> _driverAPIs;
    QList<QString> _inputDevices;
    QList<QString> _outputDevices;
    QList<QString> _sampleRates;
    QList<QString> _frameBuffers;
};

} // namespace Engine

#endif // AUDIOENGINE_H
