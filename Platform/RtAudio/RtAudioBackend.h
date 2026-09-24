#pragma once

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

    std::string lastError() const override {
        return _lastError;
    }

private:
    static int rtCallback( void* output, void* input, unsigned int frames,
                           double streamTime, unsigned int status, void* userData );

    std::vector<AudioDeviceInfo> devices( bool wantInput ) const;
    void selectUsableApi();

    std::unique_ptr<RtAudio> _audio;
    ProcessCallback _callback;
    std::string _lastError;
};

} // namespace Platform
