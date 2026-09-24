#include <Models/TunerModel.h>

#include <cmath>

#include <Core/Analysis/NoteHelper.h>
#include <Engine/Audio/AudioEngine.h>

namespace Models {

namespace {

constexpr double IN_TUNE_CENTS = 5.0;

}

TunerModel::TunerModel( Engine::AudioEngine& engine, QObject* parent ) :
    QObject( parent ),
    _engine( engine ) {

    connect( &_engine, &Engine::AudioEngine::pitchChanged, this, &TunerModel::applyReading );
}

bool TunerModel::isEnabled() const {
    return _enabled;
}

void TunerModel::setEnabled( bool enabled ) {
    if ( enabled == _enabled ) {
        return;
    }
    _enabled = enabled;
    _engine.setTunerEnabled( enabled );
    emit enabledChanged();
}

bool TunerModel::isVoiced() const {
    return _voiced;
}

QString TunerModel::noteName() const {
    return _noteName;
}

int TunerModel::octave() const {
    return _octave;
}

double TunerModel::cents() const {
    return _cents;
}

double TunerModel::frequency() const {
    return _frequency;
}

bool TunerModel::isInTune() const {
    return _voiced && std::fabs( _cents ) <= IN_TUNE_CENTS;
}

void TunerModel::applyReading( bool voiced, float frequency, int midiNote, float cents ) {
    if ( !voiced ) {
        if ( !_voiced ) {
            return;
        }
        _voiced = false;
        emit readingChanged();
        return;
    }

    _voiced = true;
    _frequency = frequency;
    _cents = cents;
    _noteName = QString::fromLatin1( Core::NoteHelper::name( midiNote ) );
    _octave = Core::NoteHelper::octave( midiNote );
    emit readingChanged();
}

} // namespace Models
