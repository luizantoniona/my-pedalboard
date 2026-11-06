#include <App/Application/Home/HomePageControl.h>

#include <Engine/Audio/AudioEngine.h>

HomePageControl::HomePageControl( Engine::AudioEngine& engine, QObject* parent ) :
    QObject( parent ),
    _engine( engine ) {

    connect( &_engine, &Engine::AudioEngine::streamStateChanged, this, &HomePageControl::streamStateChanged );

    connect( &_engine, &Engine::AudioEngine::errorOccurred, this, [ this ]( const QString& message ) {
        _lastError = message;
        emit lastErrorChanged();
    } );
}

bool HomePageControl::isRunning() const {
    return _engine.isRunning();
}

QString HomePageControl::latencyText() const {
    if ( !_engine.isRunning() ) {
        return QStringLiteral( "--" );
    }
    return QStringLiteral( "%1 ms" ).arg( _engine.latencyMs(), 0, 'f', 1 );
}

QString HomePageControl::streamText() const {
    if ( !_engine.isRunning() ) {
        return QStringLiteral( "parado" );
    }
    return QStringLiteral( "%1 Hz - %2 frames" ).arg( _engine.sampleRate() ).arg( _engine.frameBuffer() );
}

QString HomePageControl::lastError() const {
    return _lastError;
}

void HomePageControl::start() {
    _lastError.clear();
    emit lastErrorChanged();
    _engine.start();
}

void HomePageControl::stop() {
    _engine.stop();
}

void HomePageControl::toggle() {
    if ( _engine.isRunning() ) {
        stop();
        return;
    }
    start();
}
