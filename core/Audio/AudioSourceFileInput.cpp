#include "AudioSourceFileInput.h"

#include <QDebug>

AudioSourceFileInput::AudioSourceFileInput() {
}

void AudioSourceFileInput::setFilePath( const QString& path ) {
    filePath = path;
}

void AudioSourceFileInput::start() {
    stop();

    // TODO: Implementar leitura de arquivo de áudio
}

void AudioSourceFileInput::stop() {

    // TODO: Implementar liberação de recursos
}

float* AudioSourceFileInput::getBuffer() {
    return buffer;
}