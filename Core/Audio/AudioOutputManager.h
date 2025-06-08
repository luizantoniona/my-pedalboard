#ifndef AUDIOOUTPUTMANAGER_H
#define AUDIOOUTPUTMANAGER_H

#include <QObject>

class AudioOutputManager : public QObject {
    Q_OBJECT

public:
    AudioOutputManager( QObject* parent = nullptr );
    ~AudioOutputManager();

    void start();
    void stop();
};

#endif // AUDIOOUTPUTMANAGER_H
