#include "AudioEngine.h"

#include <QDebug>

namespace Engine {

AudioEngine::AudioEngine( QObject* parent ) :
    QObject( parent ) {
    qInfo() << "AudioEngine::AudioEngine";
}

AudioEngine::~AudioEngine() {
    qInfo() << "AudioEngine::~AudioEngine";
    _thread.quit();
    _thread.wait();
}

QStringList AudioEngine::inputDevices() const {
    return _inputs;
}

QStringList AudioEngine::outputDevices() const {
    return _outputs;
}

void AudioEngine::initialize() {
    _worker = new AudioWorker();
    _worker->moveToThread( &_thread );

    connect( &_thread, &QThread::started, _worker, &AudioWorker::requestDevices );

    connect( _worker, &AudioWorker::devicesReady, this, [ this ]( QStringList in, QStringList out ) {
        _inputs = in;
        _outputs = out;
        emit devicesChanged();
    } );

    _thread.start();
}

void AudioEngine::start() {
    QMetaObject::invokeMethod( _worker, "start", Qt::QueuedConnection );
}

void AudioEngine::stop() {
    QMetaObject::invokeMethod( _worker, "stop", Qt::QueuedConnection );
}

void AudioEngine::setInputDevice( int index ) {
    QMetaObject::invokeMethod( _worker, "setInputDevice", Qt::QueuedConnection, Q_ARG( int, index ) );
}

void AudioEngine::setOutputDevice( int index ) {
    QMetaObject::invokeMethod( _worker, "setOutputDevice", Qt::QueuedConnection, Q_ARG( int, index ) );
}

} // namespace Engine
