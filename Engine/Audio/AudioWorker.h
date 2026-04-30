#ifndef AUDIOWORKER_H
#define AUDIOWORKER_H

#include <QObject>

#include <RtAudio.h>

namespace Engine {

class AudioWorker : public QObject {
    Q_OBJECT

public:
    explicit AudioWorker( QObject* parent = nullptr );
    ~AudioWorker();
};

} // namespace Engine

#endif // AUDIOWORKER_H
