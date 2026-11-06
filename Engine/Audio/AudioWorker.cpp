#include <Engine/Audio/AudioWorker.h>

#include <algorithm>

#include <QDebug>
#include <QThread>

#include <Core/Analysis/NoteHelper.h>
#include <Core/Utils/DenormalGuard.h>
#include <Platform/RtAudio/RtAudioBackend.h>

namespace Engine {

namespace {

constexpr int ANALYSIS_INTERVAL_MS = 33;

}

AudioWorker::AudioWorker( QObject* parent ) :
    AudioWorker( std::unique_ptr<Platform::AudioBackend>(), parent ) {
}

AudioWorker::AudioWorker( std::unique_ptr<Platform::AudioBackend> backend, QObject* parent ) :
    QObject( parent ),
    _backend( std::move( backend ) ) {

    if ( _backend ) {
        attachCallback();
    }

    _analysisTimer = new QTimer( this );
    _analysisTimer->setInterval( ANALYSIS_INTERVAL_MS );
    connect( _analysisTimer, &QTimer::timeout, this, &AudioWorker::pollAnalysis );
}

AudioWorker::~AudioWorker() = default;

Core::AudioGraph& AudioWorker::graph() {
    return _graph;
}

void AudioWorker::initialize() {
    if ( !_backend ) {
        _backend = std::make_unique<Platform::RtAudioBackend>();
        attachCallback();
    }

    QStringList apiNames;
    for ( const std::string& api : _backend->apis() ) {
        apiNames << QString::fromStdString( api );
    }
    emit apisReady( apiNames, QString::fromStdString( _backend->currentApi() ) );

    refreshDevices();
    _analysisTimer->start();

    qInfo() << "AudioWorker: API" << QString::fromStdString( _backend->currentApi() )
            << "- entradas" << _inputs.size() << "- saidas" << _outputs.size();
}

void AudioWorker::shutdown() {
    if ( _analysisTimer ) {
        _analysisTimer->stop();
    }
    stop();

    _backend.reset();
}

void AudioWorker::refreshDevices() {
    if ( !_backend ) {
        return;
    }

    _inputs = _backend->inputDevices();
    _outputs = _backend->outputDevices();

    QStringList inputNames;
    for ( const Platform::AudioDeviceInfo& device : _inputs ) {
        inputNames << QString::fromStdString( device.name );
    }

    QStringList outputNames;
    for ( const Platform::AudioDeviceInfo& device : _outputs ) {
        outputNames << QString::fromStdString( device.name );
    }

    if ( _inputIndex < 0 && !_inputs.empty() ) {
        _inputIndex = 0;
    }
    if ( _outputIndex < 0 && !_outputs.empty() ) {
        _outputIndex = 0;
    }

    QList<int> rates;
    if ( _outputIndex >= 0 && _outputIndex < static_cast<int>( _outputs.size() ) ) {
        for ( unsigned int rate : _outputs[ _outputIndex ].sampleRates ) {
            rates << static_cast<int>( rate );
        }
    }
    if ( rates.isEmpty() ) {
        rates = { 44100, 48000, 88200, 96000 };
    }

    emit devicesReady( inputNames, outputNames, rates );
}

void AudioWorker::start() {
    if ( _running || !_backend ) {
        return;
    }

    if ( _inputIndex < 0 || _inputIndex >= static_cast<int>( _inputs.size() ) ) {
        emit errorOccurred( QStringLiteral( "Nenhum dispositivo de entrada selecionado" ) );
        return;
    }
    if ( _outputIndex < 0 || _outputIndex >= static_cast<int>( _outputs.size() ) ) {
        emit errorOccurred( QStringLiteral( "Nenhum dispositivo de saida selecionado" ) );
        return;
    }

    _requested.inputDeviceId = _inputs[ _inputIndex ].id;
    _requested.outputDeviceId = _outputs[ _outputIndex ].id;

    _granted = _backend->open( _requested );

    if ( !_backend->isOpen() ) {
        const std::string error = _backend->lastError();
        emit errorOccurred( error.empty()
                                ? QStringLiteral( "Falha ao abrir o stream de audio" )
                                : QString::fromStdString( error ) );
        publishStreamState( false );
        return;
    }

    _leftBuffer.assign( _granted.frameBuffer, 0.0f );
    _rightBuffer.assign( _granted.frameBuffer, 0.0f );

    _graph.prepare( _granted.sampleRate, _granted.frameBuffer );
    _inputMeter.prepare( _granted.sampleRate );
    _outputMeter.prepare( _granted.sampleRate );
    _pitchDetector.prepare( _granted.sampleRate );
    _analysisWindow.assign( _pitchDetector.windowSize(), 0.0f );
    _tunerFeed.clear();

    _running = true;
    publishStreamState( true );
}

void AudioWorker::stop() {
    if ( !_running || !_backend ) {
        return;
    }
    _running = false;
    _backend->close();
    _inputMeter.reset();
    _outputMeter.reset();
    publishStreamState( false );
}

void AudioWorker::setDriverApi( QString api ) {
    if ( !_backend ) {
        return;
    }

    const bool wasRunning = _running;
    stop();

    if ( !_backend->setApi( api.toStdString() ) ) {
        emit errorOccurred( QStringLiteral( "API de audio indisponivel: %1" ).arg( api ) );
        return;
    }

    attachCallback();

    _inputIndex = -1;
    _outputIndex = -1;
    refreshDevices();

    if ( wasRunning ) {
        start();
    }
}

void AudioWorker::setInputDevice( int index ) {
    if ( index < 0 || index >= static_cast<int>( _inputs.size() ) || index == _inputIndex ) {
        return;
    }
    _inputIndex = index;
    restartIfRunning();
}

void AudioWorker::setOutputDevice( int index ) {
    if ( index < 0 || index >= static_cast<int>( _outputs.size() ) || index == _outputIndex ) {
        return;
    }
    _outputIndex = index;
    restartIfRunning();
}

void AudioWorker::setSampleRate( unsigned int sampleRate ) {
    if ( sampleRate == _requested.sampleRate ) {
        return;
    }
    _requested.sampleRate = sampleRate;
    restartIfRunning();
}

void AudioWorker::setFrameBuffer( unsigned int frameBuffer ) {
    if ( frameBuffer == _requested.frameBuffer ) {
        return;
    }
    _requested.frameBuffer = frameBuffer;
    restartIfRunning();
}

void AudioWorker::setTunerEnabled( bool enabled ) {
    _tunerEnabled = enabled;
    if ( !enabled ) {
        _tunerFeed.clear();
        emit pitchChanged( false, 0.0f, 0, 0.0f );
    }
}

void AudioWorker::attachCallback() {
    _backend->setCallback( [ this ]( const float* input, float* output, uint32_t frames ) {
        process( input, output, frames );
    } );
}

void AudioWorker::restartIfRunning() {
    if ( !_running ) {
        return;
    }
    stop();
    start();
}

void AudioWorker::publishStreamState( bool running ) {
    const double latency = _granted.sampleRate > 0
                               ? 2000.0 * static_cast<double>( _granted.frameBuffer ) / static_cast<double>( _granted.sampleRate )
                               : 0.0;
    emit streamChanged( running, _granted.sampleRate, _granted.frameBuffer, latency );
}

void AudioWorker::process( const float* input, float* output, uint32_t frames ) {
    const Core::DenormalGuard denormalGuard;

    const uint32_t safeFrames = std::min<uint32_t>( frames, static_cast<uint32_t>( _leftBuffer.size() ) );

    if ( input != nullptr ) {
        std::copy_n( input, safeFrames, _leftBuffer.begin() );
    } else {
        std::fill_n( _leftBuffer.begin(), safeFrames, 0.0f );
    }

    _inputMeter.process( _leftBuffer.data(), safeFrames );

    if ( _tunerEnabled ) {
        _tunerFeed.writeBlock( _leftBuffer.data(), safeFrames );
    }

    Core::AudioBuffer buffer( _leftBuffer.data(), nullptr, safeFrames, _granted.sampleRate );
    _graph.process( buffer );

    if ( !buffer.isStereo() ) {
        std::copy_n( _leftBuffer.begin(), safeFrames, _rightBuffer.begin() );
    }

    _outputMeter.process( _leftBuffer.data(), safeFrames );

    for ( uint32_t i = 0; i < safeFrames; i++ ) {
        output[ 2 * i ] = _leftBuffer[ i ];
        output[ 2 * i + 1 ] = _rightBuffer[ i ];
    }

    for ( uint32_t i = safeFrames; i < frames; i++ ) {
        output[ 2 * i ] = 0.0f;
        output[ 2 * i + 1 ] = 0.0f;
    }
}

void AudioWorker::pollAnalysis() {
    if ( !_running ) {
        return;
    }

    emit levelsChanged( _inputMeter.peak(), _outputMeter.peak(), _inputMeter.isClipping() );

    if ( !_tunerEnabled || _analysisWindow.empty() ) {
        return;
    }

    const std::size_t window = _analysisWindow.size();
    if ( _tunerFeed.available() < window ) {
        return;
    }

    while ( _tunerFeed.available() > window * 2 ) {
        _tunerFeed.pop();
    }

    if ( _tunerFeed.readBlock( _analysisWindow.data(), window ) < window ) {
        return;
    }

    const Core::PitchDetector::Result result =
        _pitchDetector.analyze( _analysisWindow.data(), static_cast<uint32_t>( window ) );

    if ( !result.voiced ) {
        emit pitchChanged( false, 0.0f, 0, 0.0f );
        return;
    }

    const Core::NoteInfo note = Core::NoteHelper::fromFrequency( result.frequency );
    emit pitchChanged( true, result.frequency, note.midiNote, note.cents );
}

} // namespace Engine
