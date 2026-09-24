#pragma once

#include <QObject>
#include <QString>

namespace Engine {
class AudioEngine;
}

namespace Models {

class TunerModel : public QObject {
    Q_OBJECT
    Q_PROPERTY( bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged )
    Q_PROPERTY( bool voiced READ isVoiced NOTIFY readingChanged )
    Q_PROPERTY( QString noteName READ noteName NOTIFY readingChanged )
    Q_PROPERTY( int octave READ octave NOTIFY readingChanged )
    Q_PROPERTY( double cents READ cents NOTIFY readingChanged )
    Q_PROPERTY( double frequency READ frequency NOTIFY readingChanged )
    Q_PROPERTY( bool inTune READ isInTune NOTIFY readingChanged )

public:
    explicit TunerModel( Engine::AudioEngine& engine, QObject* parent = nullptr );

    bool isEnabled() const;
    void setEnabled( bool enabled );

    bool isVoiced() const;
    QString noteName() const;
    int octave() const;
    double cents() const;
    double frequency() const;
    bool isInTune() const;

signals:
    void enabledChanged();
    void readingChanged();

private:
    void applyReading( bool voiced, float frequency, int midiNote, float cents );

    Engine::AudioEngine& _engine;
    QString _noteName{ QStringLiteral( "-" ) };
    double _cents{ 0.0 };
    double _frequency{ 0.0 };
    int _octave{ 0 };
    bool _enabled{ true };
    bool _voiced{ false };
};

} // namespace Models
