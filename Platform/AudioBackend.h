#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

#include <Platform/AudioDeviceInfo.h>

namespace Platform {

class StreamConfig {

public:
    unsigned int inputDeviceId{ 0 };
    unsigned int outputDeviceId{ 0 };
    unsigned int sampleRate{ 48000 };
    unsigned int frameBuffer{ 128 };
};

// One implementation per target: RtAudio on desktop, ALSA on embedded Linux,
// I2S/DMA on the microcontroller. Speaks only in Core and STL types so that
// nothing above it depends on the audio API in use.
class AudioBackend {

public:
    using ProcessCallback = std::function<void( const float* input, float* output, uint32_t frames )>;

    virtual ~AudioBackend() = default;

    virtual std::vector<std::string> apis() const = 0;
    virtual std::string currentApi() const = 0;
    virtual bool setApi( const std::string& api ) = 0;

    virtual std::vector<AudioDeviceInfo> inputDevices() const = 0;
    virtual std::vector<AudioDeviceInfo> outputDevices() const = 0;

    virtual void setCallback( ProcessCallback callback ) = 0;

    // Returns the config actually granted by the driver, which may differ from
    // the one requested — ASIO commonly imposes its own frame buffer.
    virtual StreamConfig open( const StreamConfig& requested ) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;

    virtual std::string lastError() const = 0;
};

} // namespace Platform
