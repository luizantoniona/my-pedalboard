#include <algorithm>
#include <cmath>
#include <vector>

#include <gtest/gtest.h>

#include <Core/DSP/Effect/Modulation/Chorus.h>
#include <Tests/Support/SignalGenerator.h>

namespace {

constexpr uint32_t SAMPLE_RATE = 48000;
constexpr uint32_t MAX_FRAMES = 128;

void pushBlocks( Core::Chorus& chorus, std::vector<float>& left, std::vector<float>* right = nullptr ) {
    for ( std::size_t offset = 0; offset < left.size(); offset += MAX_FRAMES ) {
        const uint32_t frames =
            static_cast<uint32_t>( std::min<std::size_t>( MAX_FRAMES, left.size() - offset ) );
        chorus.process( left.data() + offset, right != nullptr ? right->data() + offset : nullptr, frames );
    }
}

void configure( Core::Chorus& chorus, float rate, float depth, float mix ) {
    chorus.prepare( SAMPLE_RATE, MAX_FRAMES );
    chorus.setParameter( "rate", rate );
    chorus.setParameter( "depth", depth );
    chorus.setParameter( "mix", mix );
    chorus.reset();
}

float differenceEnergy( const std::vector<float>& first, const std::vector<float>& second ) {
    float energy = 0.0f;
    for ( std::size_t i = 0; i < first.size() && i < second.size(); i++ ) {
        const float difference = first[ i ] - second[ i ];
        energy += difference * difference;
    }
    return energy;
}

} // namespace

TEST( ChorusTest, SilenceStaysSilent ) {
    Core::Chorus chorus;
    configure( chorus, 1.0f, 1.0f, 1.0f );

    std::vector<float> block = TestSupport::SignalGenerator::silence( MAX_FRAMES );
    chorus.process( block.data(), nullptr, MAX_FRAMES );

    EXPECT_FLOAT_EQ( TestSupport::SignalGenerator::peak( block ), 0.0f );
}

TEST( ChorusTest, ZeroMixLeavesTheSignalAlone ) {
    Core::Chorus chorus;
    configure( chorus, 2.0f, 1.0f, 0.0f );

    const std::vector<float> source = TestSupport::SignalGenerator::sine( 220.0f, SAMPLE_RATE, SAMPLE_RATE / 4, 0.4f );
    std::vector<float> signal = source;
    pushBlocks( chorus, signal );

    for ( std::size_t i = 0; i < source.size(); i++ ) {
        ASSERT_NEAR( signal[ i ], source[ i ], 1.0e-6f ) << "at sample " << i;
    }
}

TEST( ChorusTest, WetSignalIsADelayedCopy ) {
    Core::Chorus chorus;
    configure( chorus, 1.0f, 0.0f, 1.0f );

    std::vector<float> signal = TestSupport::SignalGenerator::impulse( 2000 );
    pushBlocks( chorus, signal );

    const std::size_t expectedIndex = static_cast<std::size_t>( 9.0f * static_cast<float>( SAMPLE_RATE ) / 1000.0f );
    EXPECT_NEAR( signal[ expectedIndex ], 1.0f, 1.0e-3f );
}

TEST( ChorusTest, DepthMovesTheDelayOverTime ) {
    const std::vector<float> source = TestSupport::SignalGenerator::pluckedString( 220.0f, SAMPLE_RATE, SAMPLE_RATE, 0.4f );

    Core::Chorus still;
    configure( still, 1.0f, 0.0f, 1.0f );
    std::vector<float> stillOutput = source;
    pushBlocks( still, stillOutput );

    Core::Chorus swept;
    configure( swept, 1.0f, 1.0f, 1.0f );
    std::vector<float> sweptOutput = source;
    pushBlocks( swept, sweptOutput );

    EXPECT_GT( differenceEnergy( stillOutput, sweptOutput ), 1.0f );
}

TEST( ChorusTest, ChannelsAreModulatedOutOfPhase ) {
    Core::Chorus chorus;
    configure( chorus, 1.0f, 1.0f, 1.0f );

    const std::vector<float> source = TestSupport::SignalGenerator::sine( 330.0f, SAMPLE_RATE, SAMPLE_RATE, 0.4f );
    std::vector<float> left = source;
    std::vector<float> right = source;
    pushBlocks( chorus, left, &right );

    EXPECT_GT( differenceEnergy( left, right ), 1.0f );
}

TEST( ChorusTest, OutputStaysWithinInputRange ) {
    Core::Chorus chorus;
    configure( chorus, 10.0f, 1.0f, 0.5f );

    std::vector<float> signal = TestSupport::SignalGenerator::whiteNoise( SAMPLE_RATE, 9, 0.8f );
    pushBlocks( chorus, signal );

    EXPECT_LE( TestSupport::SignalGenerator::peak( signal ), 0.8f + 1.0e-5f );
}

TEST( ChorusTest, ProcessBeforePrepareDoesNothing ) {
    Core::Chorus chorus;

    std::vector<float> block = TestSupport::SignalGenerator::constant( 0.5f, MAX_FRAMES );
    chorus.process( block.data(), nullptr, MAX_FRAMES );

    EXPECT_FLOAT_EQ( block[ 0 ], 0.5f );
}

TEST( ChorusTest, LeavesSamplesPastTheBlockUntouched ) {
    Core::Chorus chorus;
    configure( chorus, 1.0f, 1.0f, 1.0f );

    std::vector<float> block = TestSupport::SignalGenerator::constant( 0.5f, MAX_FRAMES );
    chorus.process( block.data(), nullptr, 7 );

    EXPECT_FLOAT_EQ( block[ 7 ], 0.5f );
}

TEST( ChorusTest, ClampsParametersAndReportsDescriptors ) {
    Core::Chorus chorus;

    EXPECT_EQ( chorus.parameters().size(), 3u );
    EXPECT_EQ( chorus.parameters()[ 0 ].name, "rate" );
    EXPECT_EQ( chorus.typeName(), "Chorus" );

    chorus.setParameter( "rate", 99.0f );
    EXPECT_NEAR( chorus.parameter( "rate" ), 10.0f, 0.001f );

    chorus.setParameter( "depth", -1.0f );
    EXPECT_NEAR( chorus.parameter( "depth" ), 0.0f, 0.001f );
}
