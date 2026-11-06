#pragma once

#include <memory>
#include <vector>

#include <QObject>

#include <Core/Graph/AudioGraph.h>
#include <Engine/Audio/AudioWorker.h>
#include <Platform/Null/NullAudioBackend.h>

namespace TestSupport {

class AudioHarness {

public:
    class Reading {

    public:
        bool voiced{ false };
        float frequency{ 0.0f };
        int midiNote{ 0 };
        float cents{ 0.0f };
    };

    explicit AudioHarness( unsigned int sampleRate = 48000, unsigned int frameBuffer = 128 );
    ~AudioHarness();

    Engine::AudioWorker& worker();
    Core::AudioGraph& graph();

    std::vector<float> render( const std::vector<float>& input );

    std::vector<float> leftChannel( const std::vector<float>& interleaved ) const;
    std::vector<float> rightChannel( const std::vector<float>& interleaved ) const;

    Reading waitForPitch( int timeoutMs = 2000 );
    float waitForInputPeak( int timeoutMs = 2000 );

    unsigned int frameBuffer() const;

private:
    Platform::NullAudioBackend* _backend{ nullptr };
    std::unique_ptr<Engine::AudioWorker> _worker;
    unsigned int _sampleRate;
    unsigned int _frameBuffer;
};

} // namespace TestSupport
