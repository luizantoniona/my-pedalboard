#include "AudioEngine.h"

#include <QDebug>

AudioEngine::AudioEngine( QObject* parent ) :
    QObject( parent ) {
    qInfo() << "AudioEngine::AudioEngine";
}

AudioEngine::~AudioEngine() {
    qInfo() << "AudioEngine::~AudioEngine";
}
