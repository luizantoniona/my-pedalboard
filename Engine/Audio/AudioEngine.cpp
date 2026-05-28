#include "AudioEngine.h"

#include <QDebug>

namespace Engine {

AudioEngine::AudioEngine( QObject* parent ) :
    QObject( parent ),
    _worker( nullptr ),
    _workerThread(),
    _driverAPI( RtAudio::RTAUDIO_DUMMY ),
    _compiledAPIs(),
    _driverAPIs( {} ),
    _inputDevices( {} ),
    _outputDevices( {} ),
    _sampleRates( {} ),
    _frameBuffers( {} ) {

    qInfo() << "AudioEngine::AudioEngine";

    RtAudio::getCompiledApi( _compiledAPIs );

    // TODO: Should append RtAudio::RTAUDIO_DUMMY as None and in the first position

    for ( const RtAudio::Api api : _compiledAPIs ) {
        _driverAPIs.append( QString::fromStdString( RtAudio::getApiDisplayName( api ) ) );
    }

    qInfo() << "AudioEngine::AudioEngine [COMPILED APIS]:" << _driverAPIs;
}

AudioEngine::~AudioEngine() {
    qInfo() << "AudioEngine::~AudioEngine";
    _workerThread.quit();
    _workerThread.wait();
    qInfo() << "AudioEngine::~AudioEngine";
}

QList<QString> AudioEngine::driverAPIs() const {
    return _driverAPIs;
}

QList<QString> AudioEngine::inputDevices() const {
    return _inputDevices;
}

QList<QString> AudioEngine::outputDevices() const {
    return _outputDevices;
}

QList<QString> AudioEngine::sampleRates() const {
    return _sampleRates;
}

QList<QString> AudioEngine::frameBuffers() const {
    return _frameBuffers;
}

void AudioEngine::setDriverAPI( int index ) {
    if ( index < 0 || index >= static_cast<int>( _compiledAPIs.size() ) ) {
        return;
    }

    _driverAPI = _compiledAPIs.at( index );

    deinitialize();

    initialize();
}

void AudioEngine::setInputDevice( int index ) {
    if ( !_worker ) {
        return;
    }

    QMetaObject::invokeMethod( _worker, "setInputDevice", Qt::QueuedConnection, Q_ARG( int, index ) );
}

void AudioEngine::setOutputDevice( int index ) {
    if ( !_worker ) {
        return;
    }

    QMetaObject::invokeMethod( _worker, "setOutputDevice", Qt::QueuedConnection, Q_ARG( int, index ) );
}

void AudioEngine::setSampleRate( int index ) {
    if ( !_worker ) {
        return;
    }

    QMetaObject::invokeMethod( _worker, "setSampleRate", Qt::QueuedConnection, Q_ARG( int, index ) );
}

void AudioEngine::setFrameBuffer( int index ) {
    if ( !_worker ) {
        return;
    }

    QMetaObject::invokeMethod( _worker, "setFrameBuffer", Qt::QueuedConnection, Q_ARG( int, index ) );
}

void AudioEngine::initialize() {
    _worker = new AudioWorker( _driverAPI );
    _worker->moveToThread( &_workerThread );

    QObject::connect( &_workerThread, &QThread::started, _worker, &AudioWorker::requestDevices );

    QObject::connect( _worker, &AudioWorker::devicesReady, this, [ this ]( QStringList inputs, QStringList outputs ) {
        _inputDevices = inputs;
        _outputDevices = outputs;
        emit devicesChanged();
    } );

    QObject::connect( _worker, &AudioWorker::sampleRatesReady, this, [ this ]( const QStringList& sampleRates ) {
        _sampleRates = sampleRates;
        emit sampleRatesChanged();
    } );

    QObject::connect( _worker, &AudioWorker::frameBuffersReady, this, [ this ]( const QStringList& frameBuffers ) {
        _frameBuffers = frameBuffers;
        emit frameBuffersChanged();
    } );

    _workerThread.start();
}

void AudioEngine::deinitialize() {
    if ( !_worker ) {
        return;
    }

    QMetaObject::invokeMethod( _worker, "stop", Qt::BlockingQueuedConnection );

    _workerThread.quit();
    _workerThread.wait();
    _worker = nullptr;

    _inputDevices.clear();
    _outputDevices.clear();
    _sampleRates.clear();
    _frameBuffers.clear();

    emit devicesChanged();
    emit sampleRatesChanged();
    emit frameBuffersChanged();
}

void AudioEngine::start() {
    QMetaObject::invokeMethod( _worker, "start", Qt::QueuedConnection );
}

void AudioEngine::stop() {
    QMetaObject::invokeMethod( _worker, "stop", Qt::QueuedConnection );
}

} // namespace Engine
