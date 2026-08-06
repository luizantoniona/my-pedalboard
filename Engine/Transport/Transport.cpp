#include "Transport.h"

namespace Engine {

Transport::Transport( QObject* parent ) : QObject( parent ) {
}

Transport::State Transport::state() const {
    return _state.load( std::memory_order_relaxed );
}

double Transport::bpm() const {
    return _bpm.load( std::memory_order_relaxed );
}

bool Transport::looping() const {
    return _looping.load( std::memory_order_relaxed );
}

uint64_t Transport::positionSamples() const {
    return _position.load( std::memory_order_relaxed );
}

void Transport::setBpm( double bpm ) {
    _bpm.store( bpm, std::memory_order_relaxed );
    emit bpmChanged();
}

void Transport::setLooping( bool looping ) {
    _looping.store( looping, std::memory_order_relaxed );
    emit loopingChanged();
}

void Transport::play() {
    _state.store( State::Playing, std::memory_order_relaxed );
    emit stateChanged();
}

void Transport::stop() {
    _state.store( State::Stopped, std::memory_order_relaxed );
    emit stateChanged();
}

void Transport::record() {
    _state.store( State::Recording, std::memory_order_relaxed );
    emit stateChanged();
}

void Transport::rewind() {
    _position.store( 0, std::memory_order_relaxed );
    emit positionChanged( 0 );
}

void Transport::advance( uint32_t frames ) {
    if ( _state.load( std::memory_order_relaxed ) == State::Stopped )
        return;
    _position.fetch_add( frames, std::memory_order_relaxed );
}

} // namespace Engine
