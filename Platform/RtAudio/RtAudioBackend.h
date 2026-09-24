#pragma once

#include <atomic>
#include <memory>

#include <Platform/AudioBackend.h>

class RtAudio;

namespace Platform {

// RtAudio is held behind a pointer so that <RtAudio.h> does not leak into
// every translation unit that reaches the engine.
class RtAudioBackend : public AudioBackend {

public:
    RtAudioBackend();
    ~RtAudioBackend() override;

    std::vector<std::string> apis() const override;
    std::string currentApi() const override;
    bool setApi( const std::string& api ) override;

    std::vector<AudioDeviceInfo> inputDevices() const override;
    std::vector<AudioDeviceInfo> outputDevices() const override;

    void setCallback( ProcessCallback callback ) override;

    StreamConfig open( const StreamConfig& requested ) override;
    void close() override;
    bool isOpen() const override;

    std::string lastError() const override;

private:
    static int rtCallback( void* output, void* input, unsigned int frames,
                           double streamTime, unsigned int status, void* userData );

    std::vector<AudioDeviceInfo> devices( bool wantInput ) const;
    void selectUsableApi();
    void attachErrorHandler();

    std::unique_ptr<RtAudio> _audio;
    ProcessCallback _callback;
    std::string _lastError;

    // RtAudio may report from the audio thread once the stream is running, so
    // nothing heavier than an atomic flag may be touched while it is open.
    std::atomic<bool> _streamOpen{ false };
    std::atomic<bool> _streamFaulted{ false };
};

} // namespace Platform
