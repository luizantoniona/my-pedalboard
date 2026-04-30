#include "AudioEngine.h"

#include <QDebug>

namespace Engine {

AudioEngine::AudioEngine( QObject* parent ) :
    QObject( parent ) {
    qInfo() << "AudioEngine::AudioEngine";
}

AudioEngine::~AudioEngine() {
    qInfo() << "AudioEngine::~AudioEngine";
}

} // namespace Engine
