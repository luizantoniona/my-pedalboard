#pragma once

#include <QObject>
#include <QVector>
#include <vector>

#include "AudioDevice.h"
#include "AudioGraph.h"

namespace Engine {

class AudioWorker : public QObject {
    Q_OBJECT

public:
    explicit AudioWorker( QObject* parent = nullptr );
    ~AudioWorker();

    AudioGraph& graph();

public slots:
    void start();
    void stop();
    void requestDevices();
    void setInputDevice( int index );
    void setOutputDevice( int index );
    void setSampleRate( unsigned int sampleRate );
    void setFrameBuffer( unsigned int frameBuffer );

    unsigned int sampleRate() const;
    unsigned int frameBuffer() const;

signals:
    void devicesReady( QStringList inputs, QStringList outputs );
    void error( QString message );

private:
    void process( const float* in, float* out, uint32_t frames );

    AudioDevice _device;
    AudioGraph _graph;

    QVector<unsigned int> _inputIds;
    QVector<unsigned int> _outputIds;

    unsigned int _inputId{ 0 };
    unsigned int _outputId{ 0 };
    unsigned int _sampleRate{ 48000 };
    unsigned int _frameBuffer{ 256 };
    bool _isRunning{ false };

    std::vector<float> _leftBuf;
    std::vector<float> _rightBuf;
};

} // namespace Engine
