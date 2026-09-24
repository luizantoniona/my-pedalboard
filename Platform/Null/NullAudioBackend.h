#pragma once

#include <Platform/AudioBackend.h>

namespace Platform {

// Backend with no hardware behind it: the graph can be driven block by block
// from a test. Keeps DSP testable without a sound card and without Qt.
class NullAudioBackend : public AudioBackend {

public:
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
        return {};
    }

    // Drives one block through the registered callback
    void renderBlock( const float* input, float* output, uint32_t frames );

private:
    ProcessCallback _callback;
    StreamConfig _config;
    bool _open{ false };
};

} // namespace Platform
