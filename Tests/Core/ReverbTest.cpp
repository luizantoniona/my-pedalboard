#include <algorithm>
#include <cmath>
#include <vector>

#include <gtest/gtest.h>

#include <Core/DSP/Effect/Reverb/Reverb.h>
#include <Tests/Support/SignalGenerator.h>

namespace {

constexpr uint32_t SAMPLE_RATE = 48000;
constexpr uint32_t MAX_FRAMES = 128;

void pushBlocks( Core::Reverb& reverb, std::vector<float>& left, std::vector<float>& right ) {
    for ( std::size_t offset = 0; offset < left.size(); offset += MAX_FRAMES ) {
        const uint32_t frames =
            static_cast<uint32_t>( std::min<std::size_t>( MAX_FRAMES, left.size() - offset ) );
        reverb.process( left.data() + offset, right.data() + offset, frames );
    }
}

void configure( Core::Reverb& reverb, float size, float damping, float mix ) {
    reverb.prepare( SAMPLE_RATE, MAX_FRAMES );
    reverb.setParameter( "size", size );
    reverb.setParameter( "damping", damping );
    reverb.setParameter( "mix", mix );
    reverb.reset();
}

float tailEnergyAfterImpulse( float size, float damping ) {
    Core::Reverb reverb;
    configure( reverb, size, damping, 1.0f );

    std::vector<float> left = TestSupport::SignalGenerator::impulse( SAMPLE_RATE * 2 );
    std::vector<float> right = left;
    pushBlocks( reverb, left, right );

    float energy = 0.0f;
    for ( std::size_t i = SAMPLE_RATE; i < left.size(); i++ ) {
        energy += left[ i ] * left[ i ];
    }
    return energy;
}

} // namespace

TEST( ReverbTest, SilenceStaysSilent ) {
    Core::Reverb reverb;
    configure( reverb, 0.9f, 0.5f, 1.0f );

    std::vector<float> left = TestSupport::SignalGenerator::silence( MAX_FRAMES );
    std::vector<float> right = left;
    reverb.process( left.data(), right.data(), MAX_FRAMES );

    EXPECT_FLOAT_EQ( TestSupport::SignalGenerator::peak( left ), 0.0f );
    EXPECT_FLOAT_EQ( TestSupport::SignalGenerator::peak( right ), 0.0f );
}

TEST( ReverbTest, ImpulseLeavesATailAfterTheDrySignalIsGone ) {
    EXPECT_GT( tailEnergyAfterImpulse( 0.8f, 0.3f ), 0.0f );
}

TEST( ReverbTest, LargerSizeRingsLonger ) {
    EXPECT_GT( tailEnergyAfterImpulse( 1.0f, 0.3f ), tailEnergyAfterImpulse( 0.0f, 0.3f ) );
}

TEST( ReverbTest, MoreDampingDarkensTheTail ) {
    EXPECT_GT( tailEnergyAfterImpulse( 0.8f, 0.0f ), tailEnergyAfterImpulse( 0.8f, 1.0f ) );
}

TEST( ReverbTest, TailIsStereo ) {
    Core::Reverb reverb;
    configure( reverb, 0.8f, 0.3f, 1.0f );

    std::vector<float> left = TestSupport::SignalGenerator::impulse( SAMPLE_RATE );
    std::vector<float> right = left;
    pushBlocks( reverb, left, right );

    float difference = 0.0f;
    for ( std::size_t i = 2000; i < left.size(); i++ ) {
        difference += std::fabs( left[ i ] - right[ i ] );
    }
    EXPECT_GT( difference, 0.0f );
}

TEST( ReverbTest, ZeroMixLeavesTheSignalAlone ) {
    Core::Reverb reverb;
    configure( reverb, 0.9f, 0.5f, 0.0f );

    const std::vector<float> source = TestSupport::SignalGenerator::sine( 220.0f, SAMPLE_RATE, SAMPLE_RATE / 4, 0.4f );
    std::vector<float> left = source;
    std::vector<float> right = source;
    pushBlocks( reverb, left, right );

    for ( std::size_t i = 0; i < source.size(); i++ ) {
        ASSERT_NEAR( left[ i ], source[ i ], 1.0e-6f ) << "at sample " << i;
    }
}

TEST( ReverbTest, LoudNoiseAtMaximumSizeStaysBounded ) {
    Core::Reverb reverb;
    configure( reverb, 1.0f, 0.0f, 1.0f );

    std::vector<float> left = TestSupport::SignalGenerator::whiteNoise( SAMPLE_RATE * 3, 5, 0.5f );
    std::vector<float> right = left;
    pushBlocks( reverb, left, right );

    EXPECT_TRUE( std::isfinite( TestSupport::SignalGenerator::peak( left ) ) );
    EXPECT_LT( TestSupport::SignalGenerator::peak( left ), 4.0f );
}

TEST( ReverbTest, WorksOnAMonoBlock ) {
    Core::Reverb reverb;
    configure( reverb, 0.8f, 0.3f, 1.0f );

    std::vector<float> mono = TestSupport::SignalGenerator::impulse( SAMPLE_RATE / 2 );
    for ( std::size_t offset = 0; offset < mono.size(); offset += MAX_FRAMES ) {
        const uint32_t frames =
            static_cast<uint32_t>( std::min<std::size_t>( MAX_FRAMES, mono.size() - offset ) );
        reverb.process( mono.data() + offset, nullptr, frames );
    }

    EXPECT_GT( TestSupport::SignalGenerator::rms( mono ), 0.0f );
}

TEST( ReverbTest, ProcessBeforePrepareDoesNothing ) {
    Core::Reverb reverb;

    std::vector<float> block = TestSupport::SignalGenerator::constant( 0.5f, MAX_FRAMES );
    reverb.process( block.data(), nullptr, MAX_FRAMES );

    EXPECT_FLOAT_EQ( block[ 0 ], 0.5f );
}

TEST( ReverbTest, ClampsParametersAndReportsDescriptors ) {
    Core::Reverb reverb;

    EXPECT_EQ( reverb.parameters().size(), 3u );
    EXPECT_EQ( reverb.parameters()[ 0 ].name, "size" );
    EXPECT_EQ( reverb.typeName(), "Reverb" );

    reverb.setParameter( "size", 4.0f );
    EXPECT_NEAR( reverb.parameter( "size" ), 1.0f, 0.001f );

    reverb.setParameter( "mix", -1.0f );
    EXPECT_NEAR( reverb.parameter( "mix" ), 0.0f, 0.001f );
}
