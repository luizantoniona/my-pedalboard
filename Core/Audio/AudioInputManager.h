#ifndef AUDIOINPUTMANAGER_H
#define AUDIOINPUTMANAGER_H

#include <thread>

class AudioInputManager {
public:
    AudioInputManager();
    ~AudioInputManager();

    void start();
    void stop();

private:
    std::thread _thread;
    std::atomic<bool> _isRunning;
};

#endif // AUDIOINPUTMANAGER_H
