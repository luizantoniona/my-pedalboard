#pragma once

#include <memory>

#include <QObject>

#include <App/Application/Home/HomePageControl.h>
#include <App/Application/Settings/SettingsPageControl.h>
#include <Engine/Audio/AudioEngine.h>
#include <Models/EffectChainModel.h>
#include <Models/LevelModel.h>
#include <Models/TunerModel.h>

class AppContext : public QObject {
    Q_OBJECT
    Q_PROPERTY( HomePageControl* home READ home CONSTANT )
    Q_PROPERTY( SettingsPageControl* settings READ settings CONSTANT )
    Q_PROPERTY( Models::TunerModel* tuner READ tuner CONSTANT )
    Q_PROPERTY( Models::LevelModel* levels READ levels CONSTANT )
    Q_PROPERTY( Models::EffectChainModel* chain READ chain CONSTANT )

public:
    explicit AppContext( QObject* parent = nullptr );
    ~AppContext() override;

    void initialize();

    HomePageControl* home() const;
    SettingsPageControl* settings() const;
    Models::TunerModel* tuner() const;
    Models::LevelModel* levels() const;
    Models::EffectChainModel* chain() const;

private:
    Engine::AudioEngine _engine;

    std::unique_ptr<Models::TunerModel> _tuner;
    std::unique_ptr<Models::LevelModel> _levels;
    std::unique_ptr<Models::EffectChainModel> _chain;
    std::unique_ptr<HomePageControl> _home;
    std::unique_ptr<SettingsPageControl> _settings;
};
