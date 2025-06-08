#ifndef AUDIOOUTPUTMANAGER_H
#define AUDIOOUTPUTMANAGER_H

#include <thread>

class AudioOutputManager {
public:
    AudioOutputManager();
    ~AudioOutputManager();

    void start();
    void stop();

private:
    std::thread _thread;
    std::atomic<bool> _isRunning;
};

#endif // AUDIOOUTPUTMANAGER_H
