#include "AudioManager.h"

#include "AudioSourceFileInput.h"
#include "AudioSourceLiveInput.h"

void AudioManager::selectSource( AudioSourceType type ) {
    stop();

    switch ( type ) {
    case AudioSourceType::LiveInput:
        currentSource = std::make_unique<AudioSourceLiveInput>();
        break;
    case AudioSourceType::FileInput:
        currentSource = std::make_unique<AudioSourceFileInput>();
        break;
    }
}

void AudioManager::start() {
    if ( currentSource )
        currentSource->start();
}

void AudioManager::stop() {
    if ( currentSource )
        currentSource->stop();
}

float* AudioManager::getCurrentBuffer() {
    return currentSource ? currentSource->getBuffer() : nullptr;
}

void AudioManager::addEffect( std::shared_ptr<Effect> effect ) {
    effects.push_back( effect );
}

void AudioManager::processEffects( float* buffer, size_t frames ) {
    for ( auto& effect : effects )
        effect->process( buffer, frames );
}
