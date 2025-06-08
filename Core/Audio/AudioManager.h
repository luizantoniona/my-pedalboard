#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <memory>

#include "AudioInputManager.h"
#include "AudioOutputManager.h"

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    void start();
    void stop();

    AudioInputManager* input();
    AudioOutputManager* output();

private:
    std::unique_ptr<AudioInputManager> _audioInput;
    std::unique_ptr<AudioOutputManager> _audioOutput;
};

#endif // AUDIOMANAGER_H
