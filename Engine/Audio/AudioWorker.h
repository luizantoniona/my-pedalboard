#ifndef AUDIOWORKER_H
#define AUDIOWORKER_H
#include <QObject>

#include <RtAudio.h>

class AudioWorker : public QObject {
    Q_OBJECT

public:
    explicit AudioWorker( QObject* parent = nullptr );
    ~AudioWorker();
};

#endif // AUDIOWORKER_H
