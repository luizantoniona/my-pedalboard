#ifndef AUDIOINPUTMANAGER_H
#define AUDIOINPUTMANAGER_H

#include <QObject>

class AudioInputManager : public QObject {
    Q_OBJECT

public:
    AudioInputManager( QObject* parent = nullptr );
    ~AudioInputManager();

    void start();
    void stop();
};

#endif // AUDIOINPUTMANAGER_H
