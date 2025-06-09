#include "AudioManager.h"

AudioManager::AudioManager() :
    _audioInput( std::make_unique<AudioInputManager>() ),
    _audioOutput( std::make_unique<AudioOutputManager>() ) {
}

AudioManager::~AudioManager() {
    stop();
}

void AudioManager::start() {
    _audioInput->start();
    _audioOutput->start();
}

void AudioManager::stop() {
    _audioInput->stop();
    _audioOutput->stop();
}

AudioInputManager* AudioManager::input() {
    return _audioInput.get();
}

AudioOutputManager* AudioManager::output() {
    return _audioOutput.get();
}
