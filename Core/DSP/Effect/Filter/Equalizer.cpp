#include <Core/DSP/Effect/Filter/Equalizer.h>

#include <cstring>

#include <Core/Utils/AudioMath.h>

namespace Core {

namespace {

constexpr int LEVEL_INDEX = 7;

constexpr float GAIN_LIMIT_DB = 15.0f;

constexpr float ONE_OCTAVE_BAND_Q = 1.4142f;

constexpr float BAND_FREQUENCIES[] = { 100.0f, 200.0f, 400.0f, 800.0f, 1600.0f, 3200.0f, 6400.0f };

constexpr ParameterInfo PARAMETERS[] = {
    ParameterInfo{ "band100", "100", "dB", -GAIN_LIMIT_DB, GAIN_LIMIT_DB, 0.0f },
    ParameterInfo{ "band200", "200", "dB", -GAIN_LIMIT_DB, GAIN_LIMIT_DB, 0.0f },
    ParameterInfo{ "band400", "400", "dB", -GAIN_LIMIT_DB, GAIN_LIMIT_DB, 0.0f },
    ParameterInfo{ "band800", "800", "dB", -GAIN_LIMIT_DB, GAIN_LIMIT_DB, 0.0f },
    ParameterInfo{ "band1600", "1.6k", "dB", -GAIN_LIMIT_DB, GAIN_LIMIT_DB, 0.0f },
    ParameterInfo{ "band3200", "3.2k", "dB", -GAIN_LIMIT_DB, GAIN_LIMIT_DB, 0.0f },
    ParameterInfo{ "band6400", "6.4k", "dB", -GAIN_LIMIT_DB, GAIN_LIMIT_DB, 0.0f },
    ParameterInfo{ "level", "Level", "dB", -GAIN_LIMIT_DB, GAIN_LIMIT_DB, 0.0f },
};

} // namespace

void Equalizer::process( float* left, float* right, uint32_t frames ) {
    float gainSnapshot[ BAND_COUNT ];
    for ( int i = 0; i < BAND_COUNT; i++ ) {
        gainSnapshot[ i ] = _gainsDb[ i ].load();
    }

    if ( !_hasApplied || std::memcmp( gainSnapshot, _appliedGainsDb, sizeof( gainSnapshot ) ) != 0 ) {
        redesignBands( gainSnapshot );
    }

    _levelSmoothed.setTarget( _levelLinear.load() );

    for ( uint32_t i = 0; i < frames; i++ ) {
        const float level = _levelSmoothed.next();

        float sampleLeft = left[ i ];
        for ( int band = 0; band < BAND_COUNT; band++ ) {
            sampleLeft = _left[ band ].process( sampleLeft );
        }
        left[ i ] = sampleLeft * level;

        if ( right != nullptr ) {
            float sampleRight = right[ i ];
            for ( int band = 0; band < BAND_COUNT; band++ ) {
                sampleRight = _right[ band ].process( sampleRight );
            }
            right[ i ] = sampleRight * level;
        }
    }
}

void Equalizer::prepare( uint32_t sampleRate, uint32_t maxFrames ) {
    _sampleRate.store( sampleRate );
    _hasApplied = false;
    _levelSmoothed.prepare( sampleRate );
    reset();
}

void Equalizer::reset() {
    for ( int band = 0; band < BAND_COUNT; band++ ) {
        _left[ band ].reset();
        _right[ band ].reset();
    }
    _levelSmoothed.snapTo( _levelLinear.load() );
}

void Equalizer::setParameter( std::string_view name, float value ) {
    for ( int i = 0; i <= LEVEL_INDEX; i++ ) {
        if ( name != PARAMETERS[ i ].name ) {
            continue;
        }

        const float clamped = AudioMath::clamp( value, PARAMETERS[ i ].minimum, PARAMETERS[ i ].maximum );
        if ( i == LEVEL_INDEX ) {
            _levelLinear.store( AudioMath::decibelToLinear( clamped ) );
        } else {
            _gainsDb[ i ].store( clamped );
        }
        return;
    }
}

float Equalizer::parameter( std::string_view name ) const {
    for ( int i = 0; i <= LEVEL_INDEX; i++ ) {
        if ( name != PARAMETERS[ i ].name ) {
            continue;
        }
        return i == LEVEL_INDEX ? AudioMath::linearToDecibel( _levelLinear.load() ) : _gainsDb[ i ].load();
    }
    return 0.0f;
}

std::span<const ParameterInfo> Equalizer::parameters() const {
    return PARAMETERS;
}

void Equalizer::redesignBands( const float* gainSnapshot ) {
    const uint32_t sampleRate = _sampleRate.load();

    for ( int band = 0; band < BAND_COUNT; band++ ) {
        _left[ band ].setPeaking( BAND_FREQUENCIES[ band ], ONE_OCTAVE_BAND_Q, gainSnapshot[ band ], sampleRate );
        _right[ band ].copyCoefficients( _left[ band ] );
    }

    std::memcpy( _appliedGainsDb, gainSnapshot, sizeof( _appliedGainsDb ) );
    _hasApplied = true;
}

} // namespace Core
