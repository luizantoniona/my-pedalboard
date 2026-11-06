#include <App/RegisterAppTypes.h>

#include <QQmlEngine>

#include <App/AppContext.h>
#include <App/Application/Home/HomePageControl.h>
#include <App/Application/Settings/SettingsPageControl.h>
#include <Models/EffectChainModel.h>
#include <Models/LevelModel.h>
#include <Models/TunerModel.h>

void RegisterAppTypes::registerTypes( AppContext& context ) {
    // --- Controls and models are reached through the context, never instantiated by QML
    qmlRegisterUncreatableType<HomePageControl>( "MyPedalboardApp", 1, 0, "HomePageControl", QStringLiteral( "Acesse via AppContext.home" ) );
    qmlRegisterUncreatableType<SettingsPageControl>( "MyPedalboardApp", 1, 0, "SettingsPageControl", QStringLiteral( "Acesse via AppContext.settings" ) );
    qmlRegisterUncreatableType<Models::TunerModel>( "MyPedalboardApp", 1, 0, "TunerModel", QStringLiteral( "Acesse via AppContext.tuner" ) );
    qmlRegisterUncreatableType<Models::LevelModel>( "MyPedalboardApp", 1, 0, "LevelModel", QStringLiteral( "Acesse via AppContext.levels" ) );
    qmlRegisterUncreatableType<Models::EffectChainModel>( "MyPedalboardApp", 1, 0, "EffectChainModel", QStringLiteral( "Acesse via AppContext.chain" ) );

    // --- Single owner of everything above
    qmlRegisterSingletonInstance( "MyPedalboardApp", 1, 0, "AppContext", &context );
}
