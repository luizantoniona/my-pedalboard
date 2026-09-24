#include <Engine/Audio/AudioEngine.h>

#include <Engine/Audio/AudioWorker.h>

namespace Engine {

AudioEngine::AudioEngine( QObject* parent ) :
    QObject( parent ) {
}

AudioEngine::~AudioEngine() {
    if ( _worker != nullptr ) {
        QMetaObject::invokeMethod( _worker, "shutdown", Qt::BlockingQueuedConnection );
    }
    _thread.quit();
    _thread.wait();

    // Deleted only after the thread is gone: shutdown() already stopped the
    // analysis timer inside that thread, so no timer is alive to be torn down here.
    delete _worker;
    _worker = nullptr;
}

void AudioEngine::initialize() {
    if ( _worker != nullptr ) {
        return;
    }

    _worker = new AudioWorker();
    _worker->moveToThread( &_thread );

    connect( &_thread, &QThread::started, _worker, &AudioWorker::initialize );

    connect( _worker, &AudioWorker::apisReady, this, [ this ]( QStringList apis, QString current ) {
        _driverApis = std::move( apis );
        _currentDriverApi = std::move( current );
        emit driverApisChanged();
    } );

    connect( _worker, &AudioWorker::devicesReady, this, [ this ]( QStringList inputs, QStringList outputs, QList<int> rates ) {
        _inputs = std::move( inputs );
        _outputs = std::move( outputs );
        _sampleRates = std::move( rates );
        emit devicesChanged();
        emit sampleRatesChanged();
    } );

    connect( _worker, &AudioWorker::streamChanged, this, [ this ]( bool running, unsigned int sampleRate, unsigned int frameBuffer, double latency ) {
        _running = running;
        _sampleRate = sampleRate;
        _frameBuffer = frameBuffer;
        _latencyMs = latency;
        emit streamStateChanged();
    } );

    connect( _worker, &AudioWorker::levelsChanged, this, &AudioEngine::levelsChanged );
    connect( _worker, &AudioWorker::pitchChanged, this, &AudioEngine::pitchChanged );
    connect( _worker, &AudioWorker::errorOccurred, this, &AudioEngine::errorOccurred );

    _thread.start();
}

QStringList AudioEngine::driverApis() const {
    return _driverApis;
}

QString AudioEngine::currentDriverApi() const {
    return _currentDriverApi;
}

QStringList AudioEngine::inputDevices() const {
    return _inputs;
}

QStringList AudioEngine::outputDevices() const {
    return _outputs;
}

QStringList AudioEngine::sampleRates() const {
    QStringList values;
    for ( int rate : _sampleRates ) {
        values << QString::number( rate );
    }
    return values;
}

QStringList AudioEngine::frameBuffers() const {
    QStringList values;
    for ( int frames : _frameBuffers ) {
        values << QString::number( frames );
    }
    return values;
}

bool AudioEngine::isRunning() const {
    return _running;
}

double AudioEngine::latencyMs() const {
    return _latencyMs;
}

unsigned int AudioEngine::sampleRate() const {
    return _sampleRate;
}

unsigned int AudioEngine::frameBuffer() const {
    return _frameBuffer;
}

void AudioEngine::start() {
    QMetaObject::invokeMethod( _worker, "start", Qt::QueuedConnection );
}

void AudioEngine::stop() {
    QMetaObject::invokeMethod( _worker, "stop", Qt::QueuedConnection );
}

void AudioEngine::setDriverApi( int index ) {
    if ( index < 0 || index >= _driverApis.size() ) {
        return;
    }
    QMetaObject::invokeMethod( _worker, "setDriverApi", Qt::QueuedConnection, Q_ARG( QString, _driverApis.at( index ) ) );
}

void AudioEngine::setInputDevice( int index ) {
    QMetaObject::invokeMethod( _worker, "setInputDevice", Qt::QueuedConnection, Q_ARG( int, index ) );
}

void AudioEngine::setOutputDevice( int index ) {
    QMetaObject::invokeMethod( _worker, "setOutputDevice", Qt::QueuedConnection, Q_ARG( int, index ) );
}

void AudioEngine::setSampleRate( int index ) {
    if ( index < 0 || index >= _sampleRates.size() ) {
        return;
    }
    QMetaObject::invokeMethod( _worker, "setSampleRate", Qt::QueuedConnection, Q_ARG( unsigned int, static_cast<unsigned int>( _sampleRates.at( index ) ) ) );
}

void AudioEngine::setFrameBuffer( int index ) {
    if ( index < 0 || index >= _frameBuffers.size() ) {
        return;
    }
    QMetaObject::invokeMethod( _worker, "setFrameBuffer", Qt::QueuedConnection, Q_ARG( unsigned int, static_cast<unsigned int>( _frameBuffers.at( index ) ) ) );
}

void AudioEngine::setTunerEnabled( bool enabled ) {
    QMetaObject::invokeMethod( _worker, "setTunerEnabled", Qt::QueuedConnection, Q_ARG( bool, enabled ) );
}

void AudioEngine::addNode( std::string id, std::shared_ptr<Core::AudioNode> node ) {
    QMetaObject::invokeMethod(
        _worker, [ worker = _worker, id = std::move( id ), node = std::move( node ) ]() mutable {
            worker->graph().addNode( id, std::move( node ) );
        },
        Qt::QueuedConnection );
}

void AudioEngine::removeNode( std::string id ) {
    QMetaObject::invokeMethod(
        _worker, [ worker = _worker, id = std::move( id ) ]() {
            worker->graph().removeNode( id );
        },
        Qt::QueuedConnection );
}

void AudioEngine::connectNodes( std::string fromId, std::string toId ) {
    QMetaObject::invokeMethod(
        _worker, [ worker = _worker, fromId = std::move( fromId ), toId = std::move( toId ) ]() {
            worker->graph().connect( fromId, toId );
        },
        Qt::QueuedConnection );
}

void AudioEngine::disconnectNodes( std::string fromId, std::string toId ) {
    QMetaObject::invokeMethod(
        _worker, [ worker = _worker, fromId = std::move( fromId ), toId = std::move( toId ) ]() {
            worker->graph().disconnect( fromId, toId );
        },
        Qt::QueuedConnection );
}

} // namespace Engine
