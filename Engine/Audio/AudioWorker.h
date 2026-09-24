#pragma once

#include <memory>
#include <vector>

#include <QObject>
#include <QStringList>
#include <QTimer>

#include <Core/Analysis/LevelMeter.h>
#include <Core/Analysis/PitchDetector.h>
#include <Core/Graph/AudioGraph.h>
#include <Core/Utils/RingBuffer.h>
#include <Platform/AudioBackend.h>

namespace Engine {

// Lives on the engine thread. Owns the backend and the graph, and is the only
// object the audio callback touches.
class AudioWorker : public QObject {
    Q_OBJECT

public:
    explicit AudioWorker( QObject* parent = nullptr );
    explicit AudioWorker( std::unique_ptr<Platform::AudioBackend> backend, QObject* parent = nullptr );
    ~AudioWorker() override;

    Core::AudioGraph& graph();

public slots:
    void initialize();
    void shutdown();

    void start();
    void stop();

    void refreshDevices();

    void setDriverApi( QString api );
    void setInputDevice( int index );
    void setOutputDevice( int index );
    void setSampleRate( unsigned int sampleRate );
    void setFrameBuffer( unsigned int frameBuffer );

    void setTunerEnabled( bool enabled );

signals:
    void apisReady( QStringList apis, QString current );
    void devicesReady( QStringList inputs, QStringList outputs, QList<int> sampleRates );
    void streamChanged( bool running, unsigned int sampleRate, unsigned int frameBuffer, double latencyMs );
    void levelsChanged( float inputPeak, float outputPeak, bool clipping );
    void pitchChanged( bool voiced, float frequency, int midiNote, float cents );
    void errorOccurred( QString message );

private:
    void process( const float* input, float* output, uint32_t frames );
    void restartIfRunning();
    void publishStreamState( bool running );
    void pollAnalysis();

    std::unique_ptr<Platform::AudioBackend> _backend;
    Core::AudioGraph _graph;
    Core::LevelMeter _inputMeter;
    Core::LevelMeter _outputMeter;
    Core::PitchDetector _pitchDetector;

    QTimer* _analysisTimer{ nullptr };

    std::vector<Platform::AudioDeviceInfo> _inputs;
    std::vector<Platform::AudioDeviceInfo> _outputs;

    std::vector<float> _leftBuffer;
    std::vector<float> _rightBuffer;
    std::vector<float> _analysisWindow;

    Core::RingBuffer<float, 8192> _tunerFeed;

    Platform::StreamConfig _requested;
    Platform::StreamConfig _granted;

    int _inputIndex{ -1 };
    int _outputIndex{ -1 };
    bool _running{ false };
    bool _tunerEnabled{ true };
};

} // namespace Engine
