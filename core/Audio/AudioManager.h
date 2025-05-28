#pragma once

#include <memory>

#include <QString>

#include "AudioSource.h"
#include "AudioSourceType.h"

class AudioManager {
public:
    void selectSource( AudioSourceType type );

    void start();

    void stop();

    float* getCurrentBuffer();

    void setFilePath( const QString& path );

private:
    std::unique_ptr<AudioSource> currentSource;
    QString filePath;
};