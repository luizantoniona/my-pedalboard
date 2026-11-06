#include <Core/DSP/Effect/Filter/Biquad.h>

#include <cmath>

namespace Core {

namespace {

constexpr float TWO_PI = 6.2831853f;
constexpr float SQRT_TWO = 1.41421356f;
constexpr float NYQUIST_MARGIN = 0.45f;

float angularFrequency( float frequencyHz, uint32_t sampleRate ) {
    const float limit = NYQUIST_MARGIN * static_cast<float>( sampleRate );
    return TWO_PI * AudioMath::clamp( frequencyHz, 1.0f, limit ) / static_cast<float>( sampleRate );
}

} // namespace

void Biquad::setPeaking( float frequencyHz, float q, float gainDb, uint32_t sampleRate ) {
    const float amplitude = std::pow( 10.0f, gainDb / 40.0f );
    const float w0 = angularFrequency( frequencyHz, sampleRate );
    const float cosine = std::cos( w0 );
    const float alpha = std::sin( w0 ) / ( 2.0f * q );

    normalizeAndStore( 1.0f + alpha * amplitude, -2.0f * cosine, 1.0f - alpha * amplitude, 1.0f + alpha / amplitude, -2.0f * cosine,
                       1.0f - alpha / amplitude );
}

void Biquad::setLowShelf( float frequencyHz, float gainDb, uint32_t sampleRate ) {
    const float amplitude = std::pow( 10.0f, gainDb / 40.0f );
    const float w0 = angularFrequency( frequencyHz, sampleRate );
    const float cosine = std::cos( w0 );
    const float twoRootAlpha = 2.0f * std::sqrt( amplitude ) * ( std::sin( w0 ) / SQRT_TWO );

    normalizeAndStore( amplitude * ( ( amplitude + 1.0f ) - ( amplitude - 1.0f ) * cosine + twoRootAlpha ),
                       2.0f * amplitude * ( ( amplitude - 1.0f ) - ( amplitude + 1.0f ) * cosine ),
                       amplitude * ( ( amplitude + 1.0f ) - ( amplitude - 1.0f ) * cosine - twoRootAlpha ),
                       ( amplitude + 1.0f ) + ( amplitude - 1.0f ) * cosine + twoRootAlpha,
                       -2.0f * ( ( amplitude - 1.0f ) + ( amplitude + 1.0f ) * cosine ),
                       ( amplitude + 1.0f ) + ( amplitude - 1.0f ) * cosine - twoRootAlpha );
}

void Biquad::setHighShelf( float frequencyHz, float gainDb, uint32_t sampleRate ) {
    const float amplitude = std::pow( 10.0f, gainDb / 40.0f );
    const float w0 = angularFrequency( frequencyHz, sampleRate );
    const float cosine = std::cos( w0 );
    const float twoRootAlpha = 2.0f * std::sqrt( amplitude ) * ( std::sin( w0 ) / SQRT_TWO );

    normalizeAndStore( amplitude * ( ( amplitude + 1.0f ) + ( amplitude - 1.0f ) * cosine + twoRootAlpha ),
                       -2.0f * amplitude * ( ( amplitude - 1.0f ) + ( amplitude + 1.0f ) * cosine ),
                       amplitude * ( ( amplitude + 1.0f ) + ( amplitude - 1.0f ) * cosine - twoRootAlpha ),
                       ( amplitude + 1.0f ) - ( amplitude - 1.0f ) * cosine + twoRootAlpha,
                       2.0f * ( ( amplitude - 1.0f ) - ( amplitude + 1.0f ) * cosine ),
                       ( amplitude + 1.0f ) - ( amplitude - 1.0f ) * cosine - twoRootAlpha );
}

void Biquad::copyCoefficients( const Biquad& other ) {
    _b0 = other._b0;
    _b1 = other._b1;
    _b2 = other._b2;
    _a1 = other._a1;
    _a2 = other._a2;
}

void Biquad::normalizeAndStore( float b0, float b1, float b2, float a0, float a1, float a2 ) {
    const float inverse = 1.0f / a0;
    _b0 = b0 * inverse;
    _b1 = b1 * inverse;
    _b2 = b2 * inverse;
    _a1 = a1 * inverse;
    _a2 = a2 * inverse;
}

} // namespace Core
