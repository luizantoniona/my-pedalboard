#pragma once

#include <QObject>

namespace Engine {
class AudioEngine;
}

namespace Models {

class LevelModel : public QObject {
    Q_OBJECT
    Q_PROPERTY( double inputPeak READ inputPeak NOTIFY levelsChanged )
    Q_PROPERTY( double outputPeak READ outputPeak NOTIFY levelsChanged )
    Q_PROPERTY( double inputDecibels READ inputDecibels NOTIFY levelsChanged )
    Q_PROPERTY( double outputDecibels READ outputDecibels NOTIFY levelsChanged )
    Q_PROPERTY( bool clipping READ isClipping NOTIFY levelsChanged )

public:
    explicit LevelModel( Engine::AudioEngine& engine, QObject* parent = nullptr );

    double inputPeak() const;
    double outputPeak() const;
    double inputDecibels() const;
    double outputDecibels() const;
    bool isClipping() const;

    Q_INVOKABLE void clearClipping();

signals:
    void levelsChanged();

private:
    void applyLevels( float inputPeak, float outputPeak, bool clipping );

    Engine::AudioEngine& _engine;
    double _inputPeak{ 0.0 };
    double _outputPeak{ 0.0 };
    bool _clipping{ false };
};

} // namespace Models
