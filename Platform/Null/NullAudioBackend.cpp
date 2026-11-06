#include <Platform/Null/NullAudioBackend.h>

namespace Platform {

std::vector<std::string> NullAudioBackend::apis() const {
    return { "Null" };
}

std::string NullAudioBackend::currentApi() const {
    return "Null";
}

bool NullAudioBackend::setApi( const std::string& api ) {
    return api == "Null";
}

std::vector<AudioDeviceInfo> NullAudioBackend::inputDevices() const {
    AudioDeviceInfo info;
    info.name = "Null Input";
    info.id = 1;
    info.channels = 1;
    info.sampleRates = { 44100, 48000, 96000 };
    return { info };
}

std::vector<AudioDeviceInfo> NullAudioBackend::outputDevices() const {
    AudioDeviceInfo info;
    info.name = "Null Output";
    info.id = 2;
    info.channels = 2;
    info.sampleRates = { 44100, 48000, 96000 };
    return { info };
}

void NullAudioBackend::setCallback( ProcessCallback callback ) {
    _callback = std::move( callback );
}

StreamConfig NullAudioBackend::open( const StreamConfig& requested ) {
    _config = requested;
    _open = true;
    return _config;
}

void NullAudioBackend::close() {
    _open = false;
}

bool NullAudioBackend::isOpen() const {
    return _open;
}

void NullAudioBackend::renderBlock( const float* input, float* output, uint32_t frames ) {
    if ( _callback ) {
        _callback( input, output, frames );
    }
}

} // namespace Platform
