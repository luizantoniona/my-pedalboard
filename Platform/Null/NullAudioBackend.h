#pragma once

#include <Platform/AudioBackend.h>

namespace Platform {

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

    void renderBlock( const float* input, float* output, uint32_t frames );

private:
    ProcessCallback _callback;
    StreamConfig _config;
    bool _open{ false };
};

} // namespace Platform
