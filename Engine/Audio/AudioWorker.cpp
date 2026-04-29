#include "AudioWorker.h"

#include <QDebug>

AudioWorker::AudioWorker( QObject* parent ) :
    QObject( parent ) {
    qInfo() << "AudioWorker::AudioWorker";
}

AudioWorker::~AudioWorker() {
    qInfo() << "AudioWorker::~AudioWorker";
}
