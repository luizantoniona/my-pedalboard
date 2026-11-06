#include <gtest/gtest.h>

#include <memory>

#include <Core/DSP/Effect/Utility/Gain.h>
#include <Core/DSP/EffectNode.h>
#include <Tests/Support/AudioHarness.h>
#include <Tests/Support/SignalGenerator.h>

using TestSupport::AudioHarness;
using TestSupport::SignalGenerator;

namespace {

std::shared_ptr<Core::EffectNode> gainNode( float decibels ) {
    auto gain = std::make_unique<Core::Gain>();
    gain->setParameter( "gainDb", decibels );
    return std::make_shared<Core::EffectNode>( std::move( gain ) );
}

} // namespace

TEST( AudioWorkerTest, PassesMonoInputToBothOutputChannels ) {
    AudioHarness harness;

    const std::vector<float> input = SignalGenerator::ramp( 512 );
    const std::vector<float> output = harness.render( input );

    ASSERT_EQ( output.size(), input.size() * 2 );
    EXPECT_EQ( harness.leftChannel( output ), input );
    EXPECT_EQ( harness.rightChannel( output ), input );
}

TEST( AudioWorkerTest, SilentInputProducesSilentOutput ) {
    AudioHarness harness;

    const std::vector<float> output = harness.render( SignalGenerator::silence( 512 ) );

    EXPECT_FLOAT_EQ( SignalGenerator::peak( output ), 0.0f );
}

TEST( AudioWorkerTest, SinePassesThroughUnchanged ) {
    AudioHarness harness;

    const std::vector<float> input = SignalGenerator::sine( 440.0f, 48000, 1024 );
    const std::vector<float> output = harness.render( input );

    EXPECT_NEAR( SignalGenerator::rms( harness.leftChannel( output ) ), SignalGenerator::rms( input ), 1.0e-6f );
}

TEST( AudioWorkerTest, NoiseIsNotAltered ) {
    AudioHarness harness;

    const std::vector<float> input = SignalGenerator::whiteNoise( 1024, 7 );
    const std::vector<float> output = harness.render( input );

    EXPECT_EQ( harness.leftChannel( output ), input );
}

TEST( AudioWorkerTest, EffectInTheGraphChangesTheOutput ) {
    AudioHarness harness;
    harness.graph().addNode( "gain", gainNode( -6.0f ) );

    const std::vector<float> input = SignalGenerator::constant( 1.0f, 128 * 80 );
    const std::vector<float> output = harness.render( input );

    EXPECT_NEAR( output.back(), 0.5011f, 1.0e-3f );
}

TEST( AudioWorkerTest, ChainedEffectsApplyInOrder ) {
    AudioHarness harness;
    harness.graph().addNode( "first", gainNode( -6.0f ) );
    harness.graph().addNode( "second", gainNode( -6.0f ) );
    harness.graph().connect( "first", "second" );

    const std::vector<float> input = SignalGenerator::constant( 1.0f, 128 * 80 );
    const std::vector<float> output = harness.render( input );

    EXPECT_NEAR( output.back(), 0.2511f, 1.0e-3f );
}

TEST( AudioWorkerTest, OutputIsIndependentOfBlockSize ) {
    const std::vector<float> input = SignalGenerator::sine( 220.0f, 48000, 4096 );

    AudioHarness small( 48000, 64 );
    AudioHarness large( 48000, 512 );

    const std::vector<float> fromSmall = small.leftChannel( small.render( input ) );
    const std::vector<float> fromLarge = large.leftChannel( large.render( input ) );

    ASSERT_EQ( fromSmall.size(), fromLarge.size() );
    for ( std::size_t i = 0; i < fromSmall.size(); i++ ) {
        EXPECT_FLOAT_EQ( fromSmall[ i ], fromLarge[ i ] ) << "divergiu na amostra " << i;
    }
}

TEST( AudioWorkerTest, InputMeterFollowsTheSignal ) {
    AudioHarness harness;

    harness.render( SignalGenerator::constant( 0.4f, 48000 ) );

    EXPECT_NEAR( harness.waitForInputPeak(), 0.4f, 1.0e-3f );
}
