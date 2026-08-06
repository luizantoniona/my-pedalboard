#pragma once

#include <QObject>
#include <QStringList>
#include <QThread>

#include <memory>
#include <string>

#include "AudioWorker.h"
#include <Engine/DSP/AudioNode.h>

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

    // Graph manipulation — thread-safe, dispatched to Engine thread
    void addNode( std::string id, std::shared_ptr<AudioNode> node );
    void removeNode( std::string id );
    void connectNodes( std::string fromId, std::string toId );
    void disconnectNodes( std::string fromId, std::string toId );

signals:
    void devicesChanged();

private:
    QThread _thread;
    AudioWorker* _worker = nullptr;

    QStringList _inputs;
    QStringList _outputs;
};

} // namespace Engine
