#include "RegisterTypes.h"

#include <QQmlEngine>

#include "Audio/AudioDeviceManager.h"
#include "Molecules/NavigationBarControl.h"
#include "Pages/HomePageControl.h"

void RegisterTypes::registerTypes() {

    registerControls();
    registerManagers();
}

void RegisterTypes::registerControls() {

    // Register Atoms Controls

    // Register Molecules Controls
    qmlRegisterType<NavigationBarControl>( "Molecules", 1, 0, "NavigationBarControl" );

    // Register Organisms Controls

    // Register Pages Controls
    qmlRegisterType<HomePageControl>( "Pages", 1, 0, "HomePageControl" );
}

void RegisterTypes::registerManagers() {

    // Audio Managers
    qmlRegisterSingletonInstance<AudioDeviceManager>( "Audio", 1, 0, "AudioDeviceManager", new AudioDeviceManager() );
}
