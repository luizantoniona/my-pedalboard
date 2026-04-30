#ifndef AUDIOCONTROL_H
#define AUDIOCONTROL_H

#include <QObject>

class AudioControl : public QObject {
    Q_OBJECT
public:
    AudioControl();
    ~AudioControl();
};

#endif // AUDIOCONTROL_H
