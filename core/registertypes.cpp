#include "registertypes.h"

#include <QQmlEngine>

#include "audio/audiomanager.h"

#include "Pages/HomePageControl.h"

void RegisterTypes::registerTypes() {

    // Register Pages Controls
    qmlRegisterType<HomePageControl>( "Pages", 1, 0, "HomePageControl" );
}
