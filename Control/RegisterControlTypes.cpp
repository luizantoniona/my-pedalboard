#include "RegisterControlTypes.h"

#include <QQmlEngine>

#include "Pages/HomePageControl.h"

void RegisterControlTypes::registerTypes() {

    // Register Pages Controls
    qmlRegisterType<HomePageControl>( "Pages", 1, 0, "HomePageControl" );
}
