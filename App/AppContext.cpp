#include <App/AppContext.h>

#include <Core/Project/PedalRegistry.h>

AppContext::AppContext( QObject* parent ) :
    QObject( parent ) {
}

AppContext::~AppContext() = default;

void AppContext::initialize() {
    _engine.initialize();

    _tuner = std::make_unique<Models::TunerModel>( _engine );
    _levels = std::make_unique<Models::LevelModel>( _engine );
    _chain = std::make_unique<Models::EffectChainModel>( _engine, Core::PedalRegistry::withBuiltinPedals() );
    _home = std::make_unique<HomePageControl>( _engine );
    _settings = std::make_unique<SettingsPageControl>( _engine );
}

HomePageControl* AppContext::home() const {
    return _home.get();
}

SettingsPageControl* AppContext::settings() const {
    return _settings.get();
}

Models::TunerModel* AppContext::tuner() const {
    return _tuner.get();
}

Models::LevelModel* AppContext::levels() const {
    return _levels.get();
}

Models::EffectChainModel* AppContext::chain() const {
    return _chain.get();
}
