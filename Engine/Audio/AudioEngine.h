#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>
#include <QStringList>
#include <QThread>

#include "AudioWorker.h"

namespace Engine {

class AudioEngine : public QObject {
    Q_OBJECT

public:
    explicit AudioEngine( QObject* parent = nullptr );
    ~AudioEngine();

    QStringList inputDevices() const;
    QStringList outputDevices() const;

    void initialize();
    void start();
    void stop();
    void setInputDevice( int index );
    void setOutputDevice( int index );

signals:
    void devicesChanged();

private:
    QThread _thread;
    AudioWorker* _worker = nullptr;

    QStringList _inputs;
    QStringList _outputs;
};

} // namespace Engine

#endif // AUDIOENGINE_H
