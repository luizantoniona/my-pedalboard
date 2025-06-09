#include "AudioInputManager.h"

AudioInputManager::AudioInputManager( QObject* parent ) :
    QObject( parent ),
    _input( nullptr ),
    _device( nullptr ) {
}

AudioInputManager::~AudioInputManager() {
}

void AudioInputManager::start() {
}

void AudioInputManager::stop() {
}

void AudioInputManager::setInputDevice( const QAudioDevice& device ) {
}
