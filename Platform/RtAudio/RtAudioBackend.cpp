#include <Platform/RtAudio/RtAudioBackend.h>

#include <algorithm>
#include <iostream>
#include <sstream>

#include <RtAudio.h>

namespace Platform {

namespace {

void discardProbeReport( RtAudioErrorType, const std::string& ) {
}

class SilencedStandardError {

public:
    SilencedStandardError() :
        _previous( std::cerr.rdbuf( _sink.rdbuf() ) ) {
    }

    ~SilencedStandardError() {
        std::cerr.rdbuf( _previous );
    }

    SilencedStandardError( const SilencedStandardError& ) = delete;
    SilencedStandardError& operator=( const SilencedStandardError& ) = delete;

private:
    std::ostringstream _sink;
    std::streambuf* _previous;
};

std::unique_ptr<RtAudio> makeDevice( RtAudio::Api api = RtAudio::UNSPECIFIED ) {
    const SilencedStandardError silenced;
    return std::make_unique<RtAudio>( api, &discardProbeReport );
}

} // namespace

RtAudioBackend::RtAudioBackend() :
    _audio( makeDevice() ) {
    attachErrorHandler();
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

        _audio.reset();
        _audio = makeDevice( candidate );
        attachErrorHandler();
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

    _streamFaulted.store( false, std::memory_order_release );
    _streamOpen.store( true, std::memory_order_release );

    return granted;
}

void RtAudioBackend::close() {
    _streamOpen.store( false, std::memory_order_release );

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

std::string RtAudioBackend::lastError() const {
    if ( _streamFaulted.load( std::memory_order_acquire ) ) {
        return "O stream de audio falhou durante a execucao";
    }
    return _lastError;
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

void RtAudioBackend::selectUsableApi() {
    if ( !inputDevices().empty() && !outputDevices().empty() ) {
        return;
    }

    const RtAudio::Api original = _audio->getCurrentApi();

    std::vector<RtAudio::Api> available;
    RtAudio::getCompiledApi( available );

    for ( RtAudio::Api candidate : available ) {
        if ( candidate == original || candidate == RtAudio::RTAUDIO_DUMMY ) {
            continue;
        }

        _audio.reset();
        _audio = makeDevice( candidate );
        attachErrorHandler();

        if ( !inputDevices().empty() && !outputDevices().empty() ) {
            return;
        }
    }

    _audio.reset();
    _audio = makeDevice( original );
    attachErrorHandler();
}

void RtAudioBackend::attachErrorHandler() {
    _audio->setErrorCallback( [ this ]( RtAudioErrorType type, const std::string& text ) {
        if ( type == RTAUDIO_WARNING ) {
            return;
        }

        if ( _streamOpen.load( std::memory_order_acquire ) ) {
            _streamFaulted.store( true, std::memory_order_release );
            return;
        }

        _lastError = text;
    } );
}

} // namespace Platform
