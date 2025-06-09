#include "AudioOutputManager.h"

AudioOutputManager::AudioOutputManager( QObject* parent ) :
    QObject( parent ),
    _output( nullptr ),
    _device( nullptr ) {
}

AudioOutputManager::~AudioOutputManager() {
}

void AudioOutputManager::start() {
}

void AudioOutputManager::stop() {
}

void AudioOutputManager::setOutputDevice( const QAudioDevice& device ) {
}
