#include "RegisterTypes.h"

#include <QQmlEngine>

#include <Audio/AudioDeviceManager.h>
#include <Audio/AudioInputManager.h>
#include <Audio/AudioOutputManager.h>

#include <Molecules/NavigationBarControl.h>
#include <Pages/HomePageControl.h>

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
    qmlRegisterSingletonInstance<AudioInputManager>( "Audio", 1, 0, "AudioInputManager", new AudioInputManager() );
    qmlRegisterSingletonInstance<AudioOutputManager>( "Audio", 1, 0, "AudioOutputManager", new AudioOutputManager() );
}
