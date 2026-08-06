#include "AudioWorker.h"

#include <QDebug>

namespace Engine {

AudioWorker::AudioWorker( QObject* parent ) : QObject( parent ) {
    _device.setCallback( [ this ]( const float* in, float* out, uint32_t frames ) {
        process( in, out, frames );
    } );
}

AudioWorker::~AudioWorker() = default;

AudioGraph& AudioWorker::graph() {
    return _graph;
}

void AudioWorker::start() {
    if ( _isRunning )
        return;
    if ( _inputId == 0 && _outputId == 0 ) {
        emit error( "No device selected" );
        return;
    }
    _leftBuf.resize( _frameBuffer );
    _rightBuf.resize( _frameBuffer );
    _device.open( _inputId, _outputId, _sampleRate, _frameBuffer );
    _isRunning = _device.isOpen();
}

void AudioWorker::stop() {
    if ( !_isRunning )
        return;
    _device.close();
    _isRunning = false;
}

void AudioWorker::requestDevices() {
    auto inputs = _device.enumerateInputs( _inputIds );
    auto outputs = _device.enumerateOutputs( _outputIds );
    emit devicesReady( inputs, outputs );
}

void AudioWorker::setInputDevice( int index ) {
    if ( index < 0 || index >= _inputIds.size() )
        return;
    _inputId = _inputIds[ index ];
    if ( _isRunning ) {
        stop();
        start();
    }
}

void AudioWorker::setOutputDevice( int index ) {
    if ( index < 0 || index >= _outputIds.size() )
        return;
    _outputId = _outputIds[ index ];
    if ( _isRunning ) {
        stop();
        start();
    }
}

void AudioWorker::setSampleRate( unsigned int sampleRate ) {
    if ( sampleRate == _sampleRate )
        return;
    _sampleRate = sampleRate;
    if ( _isRunning ) {
        stop();
        start();
    }
}

unsigned int AudioWorker::sampleRate() const {
    return _sampleRate;
}

void AudioWorker::setFrameBuffer( unsigned int frameBuffer ) {
    if ( frameBuffer == _frameBuffer )
        return;
    _frameBuffer = frameBuffer;
    if ( _isRunning ) {
        stop();
        start();
    }
}

unsigned int AudioWorker::frameBuffer() const {
    return _frameBuffer;
}

void AudioWorker::process( const float* in, float* out, uint32_t frames ) {
    // Deinterleave mono input → separate L/R working buffers
    for ( uint32_t i = 0; i < frames; i++ ) {
        _leftBuf[ i ] = in ? in[ i ] : 0.0f;
        _rightBuf[ i ] = in ? in[ i ] : 0.0f;
    }

    AudioBuffer buffer{ _leftBuf.data(), _rightBuf.data(), frames, _sampleRate };
    _graph.process( buffer );

    // Interleave L/R → stereo output
    for ( uint32_t i = 0; i < frames; i++ ) {
        out[ 2 * i ] = _leftBuf[ i ];
        out[ 2 * i + 1 ] = _rightBuf[ i ];
    }
}

} // namespace Engine
