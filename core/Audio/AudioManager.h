#pragma once

#include <memory>
#include <vector>

#include <QString>

#include "AudioSource.h"
#include "AudioSourceType.h"
#include "Effect.h"

class AudioManager {
public:
    void selectSource( AudioSourceType type );

    void start();

    void stop();

    float* getCurrentBuffer();

    void addEffect( std::shared_ptr<Effect> effect );

    void processEffects( float* buffer, size_t frames );

    void setFilePath( const QString& path );

private:
    std::unique_ptr<AudioSource> currentSource;
    std::vector<std::shared_ptr<Effect> > effects;
    QString filePath;
};
