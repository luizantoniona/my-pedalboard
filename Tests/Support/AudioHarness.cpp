#include <Tests/Support/AudioHarness.h>

#include <algorithm>

#include <QCoreApplication>
#include <QDeadlineTimer>
#include <QSignalSpy>

namespace TestSupport {

AudioHarness::AudioHarness( unsigned int sampleRate, unsigned int frameBuffer ) :
    _sampleRate( sampleRate ),
    _frameBuffer( frameBuffer ) {

    auto backend = std::make_unique<Platform::NullAudioBackend>();
    _backend = backend.get();

    _worker = std::make_unique<Engine::AudioWorker>( std::move( backend ) );
    _worker->initialize();
    _worker->setSampleRate( _sampleRate );
    _worker->setFrameBuffer( _frameBuffer );
    _worker->start();
}

AudioHarness::~AudioHarness() {
    _worker->shutdown();
}

Engine::AudioWorker& AudioHarness::worker() {
    return *_worker;
}

Core::AudioGraph& AudioHarness::graph() {
    return _worker->graph();
}

std::vector<float> AudioHarness::render( const std::vector<float>& input ) {
    std::vector<float> collected;
    collected.reserve( input.size() * 2 );

    std::vector<float> block( _frameBuffer, 0.0f );
    std::vector<float> output( _frameBuffer * 2, 0.0f );

    for ( std::size_t offset = 0; offset < input.size(); offset += _frameBuffer ) {
        const std::size_t available = std::min<std::size_t>( _frameBuffer, input.size() - offset );

        std::fill( block.begin(), block.end(), 0.0f );
        std::copy_n( input.begin() + static_cast<long>( offset ), available, block.begin() );
        std::fill( output.begin(), output.end(), 0.0f );

        _backend->renderBlock( block.data(), output.data(), _frameBuffer );

        collected.insert( collected.end(), output.begin(), output.begin() + static_cast<long>( available * 2 ) );
    }

    return collected;
}

std::vector<float> AudioHarness::leftChannel( const std::vector<float>& interleaved ) const {
    std::vector<float> channel;
    channel.reserve( interleaved.size() / 2 );
    for ( std::size_t i = 0; i < interleaved.size(); i += 2 ) {
        channel.push_back( interleaved[ i ] );
    }
    return channel;
}

std::vector<float> AudioHarness::rightChannel( const std::vector<float>& interleaved ) const {
    std::vector<float> channel;
    channel.reserve( interleaved.size() / 2 );
    for ( std::size_t i = 1; i < interleaved.size(); i += 2 ) {
        channel.push_back( interleaved[ i ] );
    }
    return channel;
}

AudioHarness::Reading AudioHarness::waitForPitch( int timeoutMs ) {
    QSignalSpy spy( _worker.get(), &Engine::AudioWorker::pitchChanged );
    QDeadlineTimer deadline( timeoutMs );

    while ( !deadline.hasExpired() ) {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 10 );

        while ( !spy.isEmpty() ) {
            const QList<QVariant> arguments = spy.takeFirst();
            if ( !arguments.at( 0 ).toBool() ) {
                continue;
            }

            Reading reading;
            reading.voiced = true;
            reading.frequency = arguments.at( 1 ).toFloat();
            reading.midiNote = arguments.at( 2 ).toInt();
            reading.cents = arguments.at( 3 ).toFloat();
            return reading;
        }
    }

    return Reading();
}

float AudioHarness::waitForInputPeak( int timeoutMs ) {
    QSignalSpy spy( _worker.get(), &Engine::AudioWorker::levelsChanged );
    QDeadlineTimer deadline( timeoutMs );

    float highest = 0.0f;
    while ( !deadline.hasExpired() ) {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 10 );

        while ( !spy.isEmpty() ) {
            const QList<QVariant> arguments = spy.takeFirst();
            highest = std::max( highest, arguments.at( 0 ).toFloat() );
        }

        if ( highest > 0.0f ) {
            return highest;
        }
    }

    return highest;
}

unsigned int AudioHarness::frameBuffer() const {
    return _frameBuffer;
}

} // namespace TestSupport
