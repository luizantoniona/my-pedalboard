#ifndef AUDIOENGINE_H
#define AUDIOENGINE_H

#include <QObject>

namespace Engine {

class AudioEngine : public QObject {
    Q_OBJECT

public:
    explicit AudioEngine( QObject* parent = nullptr );
    ~AudioEngine();
};

} // namespace Engine

#endif // AUDIOENGINE_H
