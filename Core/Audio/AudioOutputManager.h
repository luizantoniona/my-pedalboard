#ifndef AUDIOOUTPUTMANAGER_H
#define AUDIOOUTPUTMANAGER_H

#include <QAudioDevice>
#include <QAudioOutput>
#include <QObject>

class AudioOutputManager : public QObject {
    Q_OBJECT

public:
    AudioOutputManager( QObject* parent = nullptr );
    ~AudioOutputManager();

    void start();
    void stop();

public slots:
    void setOutputDevice( const QAudioDevice& device );

private:
    QAudioOutput* _output;
    QIODevice* _device;
    QAudioDevice _audioDevice;
    QAudioFormat _format;
};

#endif // AUDIOOUTPUTMANAGER_H
