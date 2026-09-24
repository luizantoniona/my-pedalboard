#include <Platform/RtAudio/RtAudioBackend.h>

#include <algorithm>

#include <RtAudio.h>

namespace Platform {

RtAudioBackend::RtAudioBackend() :
    _audio( std::make_unique<RtAudio>() ) {
    selectUsableApi();
}

RtAudioBackend::~RtAudioBackend() {
    close();
}

std::vector<std::string> RtAudioBackend::apis() const {
    std::vector<RtAudio::Api> available;
    RtAudio::getCompiledApi( available );

    std::vector<std::string> names;
    names.reserve( available.size() );
    for ( RtAudio::Api api : available ) {
        names.push_back( RtAudio::getApiDisplayName( api ) );
    }
    return names;
}

std::string RtAudioBackend::currentApi() const {
    return RtAudio::getApiDisplayName( _audio->getCurrentApi() );
}

bool RtAudioBackend::setApi( const std::string& api ) {
    std::vector<RtAudio::Api> available;
    RtAudio::getCompiledApi( available );

    for ( RtAudio::Api candidate : available ) {
        if ( RtAudio::getApiDisplayName( candidate ) != api ) {
            continue;
        }
        close();
        _audio = std::make_unique<RtAudio>( candidate );
        return true;
    }
    return false;
}

std::vector<AudioDeviceInfo> RtAudioBackend::devices( bool wantInput ) const {
    std::vector<AudioDeviceInfo> found;

    for ( unsigned int id : _audio->getDeviceIds() ) {
        const RtAudio::DeviceInfo probed = _audio->getDeviceInfo( id );
        const unsigned int channels = wantInput ? probed.inputChannels : probed.outputChannels;
        if ( channels == 0 ) {
            continue;
        }

        AudioDeviceInfo info;
        info.name = probed.name;
        info.id = id;
        info.channels = channels;
        info.sampleRates = probed.sampleRates;
        found.push_back( std::move( info ) );
    }

    return found;
}

std::vector<AudioDeviceInfo> RtAudioBackend::inputDevices() const {
    return devices( true );
}

std::vector<AudioDeviceInfo> RtAudioBackend::outputDevices() const {
    return devices( false );
}

void RtAudioBackend::setCallback( ProcessCallback callback ) {
    _callback = std::move( callback );
}

StreamConfig RtAudioBackend::open( const StreamConfig& requested ) {
    close();
    _lastError.clear();

    StreamConfig granted = requested;

    RtAudio::StreamParameters inputParams;
    RtAudio::StreamParameters outputParams;

    inputParams.deviceId = requested.inputDeviceId;
    inputParams.nChannels = 1;
    inputParams.firstChannel = 0;

    outputParams.deviceId = requested.outputDeviceId;
    outputParams.nChannels = 2;
    outputParams.firstChannel = 0;

    RtAudio::StreamOptions options;
    options.flags = RTAUDIO_MINIMIZE_LATENCY;

    // RtAudio writes the frame count it actually granted back into this
    // variable. Discarding it sizes the working buffers too small and the
    // callback then writes past their end — ASIO routinely imposes its own.
    unsigned int frames = requested.frameBuffer;

    const RtAudioErrorType status = _audio->openStream(
        requested.outputDeviceId != 0 ? &outputParams : nullptr,
        requested.inputDeviceId != 0 ? &inputParams : nullptr,
        RTAUDIO_FLOAT32, requested.sampleRate, &frames,
        &RtAudioBackend::rtCallback, this, &options );

    if ( status != RTAUDIO_NO_ERROR ) {
        _lastError = _audio->getErrorText();
        return granted;
    }

    granted.frameBuffer = frames;
    granted.sampleRate = _audio->getStreamSampleRate();

    if ( _audio->startStream() != RTAUDIO_NO_ERROR ) {
        _lastError = _audio->getErrorText();
        _audio->closeStream();
        return granted;
    }

    return granted;
}

void RtAudioBackend::close() {
    if ( _audio->isStreamRunning() ) {
        _audio->stopStream();
    }
    if ( _audio->isStreamOpen() ) {
        _audio->closeStream();
    }
}

bool RtAudioBackend::isOpen() const {
    return _audio->isStreamOpen();
}

int RtAudioBackend::rtCallback( void* output, void* input, unsigned int frames,
                                double streamTime, unsigned int status, void* userData ) {
    auto* self = static_cast<RtAudioBackend*>( userData );
    if ( self->_callback ) {
        self->_callback( static_cast<const float*>( input ),
                         static_cast<float*>( output ),
                         static_cast<uint32_t>( frames ) );
    }
    return 0;
}

// The default API can be one with no hardware attached: an installed ASIO
// driver whose interface is unplugged enumerates nothing. Fall back to the
// first compiled API that reports both an input and an output.
void RtAudioBackend::selectUsableApi() {
    if ( !inputDevices().empty() && !outputDevices().empty() ) {
        return;
    }

    std::vector<RtAudio::Api> available;
    RtAudio::getCompiledApi( available );

    for ( RtAudio::Api candidate : available ) {
        if ( candidate == _audio->getCurrentApi() || candidate == RtAudio::RTAUDIO_DUMMY ) {
            continue;
        }

        auto probe = std::make_unique<RtAudio>( candidate );
        _audio.swap( probe );

        if ( !inputDevices().empty() && !outputDevices().empty() ) {
            return;
        }

        _audio.swap( probe );
    }
}

} // namespace Platform
