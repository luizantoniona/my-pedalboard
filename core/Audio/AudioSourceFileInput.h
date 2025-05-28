#pragma once

#include <QString>

#include "AudioSource.h"

class AudioSourceFileInput : public AudioSource {
public:
    AudioSourceFileInput();

    void start() override;

    void stop() override;

    float* getBuffer() override;

    void setFilePath( const QString& path );

private:
    float* buffer = nullptr;
    size_t totalFrames = 0;
    size_t currentFrame = 0;
    QString filePath;
};