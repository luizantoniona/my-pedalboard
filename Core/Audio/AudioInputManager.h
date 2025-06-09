#ifndef AUDIOINPUTMANAGER_H
#define AUDIOINPUTMANAGER_H

#include <QAudioDevice>
#include <QAudioInput>
#include <QObject>

class AudioInputManager : public QObject {
    Q_OBJECT

public:
    AudioInputManager( QObject* parent = nullptr );
    ~AudioInputManager();

    void start();
    void stop();

public slots:
    void setInputDevice( const QAudioDevice& device );

private:
    QAudioInput* _input;
    QIODevice* _device;
    QAudioDevice _audioDevice;
    QAudioFormat _format;
};

#endif // AUDIOINPUTMANAGER_H
