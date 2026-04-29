#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>

class AudioEngine : public QObject {
    Q_OBJECT

public:
    explicit AudioEngine( QObject* parent = nullptr );
    ~AudioEngine();
};

#endif // AUDIOENGINE_H
