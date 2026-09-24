#pragma once

#include <memory>
#include <string>

#include <QList>
#include <QObject>
#include <QStringList>
#include <QThread>

#include <Core/DSP/AudioNode.h>

namespace Engine {

class AudioWorker;

// Facade on the UI thread. Owns the engine thread and the worker; every call
// below is forwarded as a queued invocation. Created and owned by AppContext,
// never a singleton: it holds a QThread and an audio stream that must be torn
// down before QGuiApplication goes away.
class AudioEngine : public QObject {
    Q_OBJECT

public:
    explicit AudioEngine( QObject* parent = nullptr );
    ~AudioEngine() override;

    void initialize();

    QStringList driverApis() const;
    QString currentDriverApi() const;
    QStringList inputDevices() const;
    QStringList outputDevices() const;
    QStringList sampleRates() const;
    QStringList frameBuffers() const;

    bool isRunning() const;
    double latencyMs() const;
    unsigned int sampleRate() const;
    unsigned int frameBuffer() const;

    void start();
    void stop();

    void setDriverApi( int index );
    void setInputDevice( int index );
    void setOutputDevice( int index );
    void setSampleRate( int index );
    void setFrameBuffer( int index );
    void setTunerEnabled( bool enabled );

    void addNode( std::string id, std::shared_ptr<Core::AudioNode> node );
    void removeNode( std::string id );
    void connectNodes( std::string fromId, std::string toId );
    void disconnectNodes( std::string fromId, std::string toId );

signals:
    void driverApisChanged();
    void devicesChanged();
    void sampleRatesChanged();
    void frameBuffersChanged();
    void streamStateChanged();
    void levelsChanged( float inputPeak, float outputPeak, bool clipping );
    void pitchChanged( bool voiced, float frequency, int midiNote, float cents );
    void errorOccurred( QString message );

private:
    QThread _thread;
    AudioWorker* _worker{ nullptr };

    QStringList _driverApis;
    QString _currentDriverApi;
    QStringList _inputs;
    QStringList _outputs;
    QList<int> _sampleRates;
    QList<int> _frameBuffers{ 64, 128, 256, 512, 1024 };

    double _latencyMs{ 0.0 };
    unsigned int _sampleRate{ 48000 };
    unsigned int _frameBuffer{ 128 };
    bool _running{ false };
};

} // namespace Engine
