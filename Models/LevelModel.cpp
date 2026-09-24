#include <Models/LevelModel.h>

#include <Core/Utils/AudioMath.h>
#include <Engine/Audio/AudioEngine.h>

namespace Models {

LevelModel::LevelModel( Engine::AudioEngine& engine, QObject* parent ) :
    QObject( parent ),
    _engine( engine ) {

    connect( &_engine, &Engine::AudioEngine::levelsChanged, this, &LevelModel::applyLevels );
}

double LevelModel::inputPeak() const {
    return _inputPeak;
}

double LevelModel::outputPeak() const {
    return _outputPeak;
}

double LevelModel::inputDecibels() const {
    return Core::AudioMath::linearToDecibel( static_cast<float>( _inputPeak ) );
}

double LevelModel::outputDecibels() const {
    return Core::AudioMath::linearToDecibel( static_cast<float>( _outputPeak ) );
}

bool LevelModel::isClipping() const {
    return _clipping;
}

void LevelModel::clearClipping() {
    if ( !_clipping ) {
        return;
    }
    _clipping = false;
    emit levelsChanged();
}

void LevelModel::applyLevels( float inputPeak, float outputPeak, bool clipping ) {
    _inputPeak = inputPeak;
    _outputPeak = outputPeak;
    if ( clipping ) {
        _clipping = true;
    }
    emit levelsChanged();
}

} // namespace Models
