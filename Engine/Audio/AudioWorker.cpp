#include "AudioWorker.h"

#include <QDebug>

namespace Engine {

AudioWorker::AudioWorker( QObject* parent ) :
    QObject( parent ) {
    qInfo() << "AudioWorker::AudioWorker";
}

AudioWorker::~AudioWorker() {
    qInfo() << "AudioWorker::~AudioWorker";
}

} // namespace Engine
