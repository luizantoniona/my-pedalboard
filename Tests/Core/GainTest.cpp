#include <gtest/gtest.h>

#include <vector>

#include <Core/DSP/Effect/Utility/Gain.h>

namespace {

std::vector<float> renderSteady( Core::Gain& gain, float input, uint32_t blocks, uint32_t frames ) {
    std::vector<float> buffer( frames, input );
    for ( uint32_t block = 0; block < blocks; block++ ) {
        std::fill( buffer.begin(), buffer.end(), input );
        gain.process( buffer.data(), nullptr, frames );
    }
    return buffer;
}

} // namespace

TEST( GainTest, SilenceStaysSilent ) {
    Core::Gain gain;
    gain.prepare( 48000, 512 );
    gain.setParameter( "gainDb", 12.0f );

    const std::vector<float> rendered = renderSteady( gain, 0.0f, 50, 512 );
    for ( float sample : rendered ) {
        EXPECT_FLOAT_EQ( sample, 0.0f );
    }
}

TEST( GainTest, UnityGainLeavesSignalUntouched ) {
    Core::Gain gain;
    gain.prepare( 48000, 512 );
    gain.setParameter( "gainDb", 0.0f );

    const std::vector<float> rendered = renderSteady( gain, 0.5f, 50, 512 );
    EXPECT_NEAR( rendered.back(), 0.5f, 1.0e-4f );
}

TEST( GainTest, ReachesRequestedGainAfterRamp ) {
    Core::Gain gain;
    gain.prepare( 48000, 512 );
    gain.setParameter( "gainDb", -6.0f );

    const std::vector<float> rendered = renderSteady( gain, 1.0f, 50, 512 );
    EXPECT_NEAR( rendered.back(), 0.5011f, 1.0e-3f );
}

TEST( GainTest, RampAvoidsStepDiscontinuity ) {
    Core::Gain gain;
    gain.prepare( 48000, 512 );
    gain.setParameter( "gainDb", 0.0f );
    renderSteady( gain, 1.0f, 50, 512 );

    gain.setParameter( "gainDb", -40.0f );
    std::vector<float> buffer( 512, 1.0f );
    gain.process( buffer.data(), nullptr, 512 );

    EXPECT_GT( buffer.front(), 0.9f );
}

TEST( GainTest, DisabledModuleReportsItself ) {
    Core::Gain gain;
    EXPECT_TRUE( gain.isEnabled() );

    gain.setEnabled( false );
    EXPECT_FALSE( gain.isEnabled() );
}

TEST( GainTest, ClampsOutOfRangeParameter ) {
    Core::Gain gain;
    gain.prepare( 48000, 512 );
    gain.setParameter( "gainDb", 999.0f );

    EXPECT_NEAR( gain.parameter( "gainDb" ), 24.0f, 1.0e-3f );
}
