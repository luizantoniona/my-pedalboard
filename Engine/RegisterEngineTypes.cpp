#include "RegisterEngineTypes.h"

#include <QObject>

#include <RtAudio.h>

void RegisterEngineTypes::registerTypes() {

    qRegisterMetaType<RtAudio::Api>();
}
