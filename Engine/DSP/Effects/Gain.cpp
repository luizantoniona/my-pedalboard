#include "Gain.h"

namespace Engine {

void Gain::process( float* left, float* right, uint32_t frames ) {
    const float g = _gain.load();
    for ( uint32_t i = 0; i < frames; i++ ) {
        left[ i ] *= g;
        right[ i ] *= g;
    }
}

void Gain::setParameter( std::string_view name, float value ) {
    if ( name == "gainDb" )
        _gain.store( AudioMath::dbToLinear( value ) );
    else if ( name == "gainLinear" )
        _gain.store( value );
}

float Gain::getParameter( std::string_view name ) const {
    if ( name == "gainDb" )
        return AudioMath::linearToDb( _gain.load() );
    if ( name == "gainLinear" )
        return _gain.load();
    return 0.0f;
}

} // namespace Engine
