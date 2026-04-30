#include "RegisterControlTypes.h"

#include <QQmlEngine>

#include "AudioControl.h"

void RegisterControlTypes::registerTypes() {

    // Register Pages Controls
    qmlRegisterType<AudioControl>( "Control", 1, 0, "AudioControl" );
}
