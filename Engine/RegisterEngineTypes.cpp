#include "RegisterEngineTypes.h"

#include <QQmlEngine>

#include "Audio/AudioEngine.h"

void RegisterEngineTypes::registerTypes() {

    // Register Audio Engine
    qmlRegisterType<AudioEngine>( "Engine", 1, 0, "AudioEngine" );
}
