#include <gtest/gtest.h>

#include <cmath>
#include <memory>
#include <vector>

#include <Core/DSP/Effect/Distortion/Overdrive.h>
#include <Core/DSP/Effect/Dynamics/NoiseGate.h>
#include <Core/DSP/Effect/Utility/Gain.h>
#include <Core/DSP/EffectNode.h>
#include <Core/Project/NodeRegistry.h>
#include <Tests/Support/AudioHarness.h>
#include <Tests/Support/SignalGenerator.h>

using TestSupport::AudioHarness;
using TestSupport::SignalGenerator;

namespace {

std::shared_ptr<Core::EffectNode> nodeOf( std::unique_ptr<Core::DSPModule> effect ) {
    return std::make_shared<Core::EffectNode>( std::move( effect ) );
}

std::shared_ptr<Core::EffectNode> gainNode( float decibels ) {
    auto gain = std::make_unique<Core::Gain>();
    gain->setParameter( "gainDb", decibels );
    return nodeOf( std::move( gain ) );
}

std::shared_ptr<Core::EffectNode> overdriveNode( float drive ) {
    auto overdrive = std::make_unique<Core::Overdrive>();
    overdrive->setParameter( "drive", drive );
    overdrive->setParameter( "tone", 1.0f );
    overdrive->setParameter( "level", 0.0f );
    overdrive->setParameter( "mix", 1.0f );
    return nodeOf( std::move( overdrive ) );
}

} // namespace

TEST( EffectChainTest, RegistryBuildsEveryAdvertisedType ) {
    const Core::NodeRegistry& registry = Core::NodeRegistry::withBuiltinTypes();

    const std::vector<std::string> names = registry.typeNames();
    ASSERT_FALSE( names.empty() );

    for ( const std::string& name : names ) {
        const std::unique_ptr<Core::DSPModule> effect = registry.create( name );
        ASSERT_NE( effect, nullptr ) << name;
        EXPECT_EQ( effect->typeName(), name );
    }
}

TEST( EffectChainTest, SerialChainRunsEveryStage ) {
    AudioHarness harness;

    harness.graph().addNode( "first", gainNode( -6.0f ) );
    harness.graph().addNode( "second", gainNode( -6.0f ) );
    harness.graph().setSerialChain( { "first", "second" } );

    const std::vector<float> input = SignalGenerator::constant( 1.0f, 48000 );
    const std::vector<float> output = harness.leftChannel( harness.render( input ) );

    EXPECT_NEAR( output.back(), 0.2511f, 1.0e-3f );
}

TEST( EffectChainTest, ReorderingChangesTheAudibleResult ) {
    const std::vector<float> input = SignalGenerator::sine( 440.0f, 48000, 48000, 0.05f );

    const auto renderWithOrder = []( const std::vector<std::string>& order, const std::vector<float>& signal ) {
        AudioHarness harness;
        harness.graph().addNode( "boost", gainNode( 18.0f ) );
        harness.graph().addNode( "drive", overdriveNode( 20.0f ) );
        harness.graph().setSerialChain( order );
        const std::vector<float> output = harness.leftChannel( harness.render( signal ) );
        return SignalGenerator::rms( std::vector<float>( output.end() - 4800, output.end() ) );
    };

    const float boostFirst = renderWithOrder( { "boost", "drive" }, input );
    const float driveFirst = renderWithOrder( { "drive", "boost" }, input );

    EXPECT_GT( std::fabs( boostFirst - driveFirst ), 0.01f );
}

TEST( EffectChainTest, RemovingTheMiddlePedalKeepsTheRestInOrder ) {
    AudioHarness harness;

    harness.graph().addNode( "first", gainNode( -6.0f ) );
    harness.graph().addNode( "middle", gainNode( -6.0f ) );
    harness.graph().addNode( "last", gainNode( -6.0f ) );
    harness.graph().setSerialChain( { "first", "middle", "last" } );

    harness.graph().removeNode( "middle" );
    harness.graph().setSerialChain( { "first", "last" } );

    ASSERT_EQ( harness.graph().edges().size(), 1u );

    const std::vector<float> output =
        harness.leftChannel( harness.render( SignalGenerator::constant( 1.0f, 48000 ) ) );

    EXPECT_NEAR( output.back(), 0.2511f, 1.0e-3f );
}

TEST( EffectChainTest, BypassTakesAPedalOutOfTheChain ) {
    AudioHarness harness;

    const std::shared_ptr<Core::EffectNode> muted = gainNode( -60.0f );
    harness.graph().addNode( "muted", muted );
    harness.graph().setSerialChain( { "muted" } );

    const std::vector<float> input = SignalGenerator::constant( 1.0f, 4096 );

    const std::vector<float> attenuated = harness.leftChannel( harness.render( input ) );
    EXPECT_LT( std::fabs( attenuated.back() ), 0.01f );

    muted->effect()->setEnabled( false );

    const std::vector<float> bypassed = harness.leftChannel( harness.render( input ) );
    EXPECT_FLOAT_EQ( bypassed.back(), 1.0f );
}

TEST( EffectChainTest, ParameterChangeReachesTheRunningChain ) {
    AudioHarness harness;

    const std::shared_ptr<Core::EffectNode> gain = gainNode( 0.0f );
    harness.graph().addNode( "gain", gain );
    harness.graph().setSerialChain( { "gain" } );

    const std::vector<float> input = SignalGenerator::constant( 1.0f, 48000 );

    const std::vector<float> unity = harness.leftChannel( harness.render( input ) );
    EXPECT_NEAR( unity.back(), 1.0f, 1.0e-3f );

    gain->effect()->setParameter( "gainDb", -12.0f );

    const std::vector<float> quieter = harness.leftChannel( harness.render( input ) );
    EXPECT_NEAR( quieter.back(), 0.2512f, 1.0e-3f );
}

TEST( EffectChainTest, GateAndOverdriveRunTogetherWithoutBlowingUp ) {
    AudioHarness harness;

    auto gate = std::make_unique<Core::NoiseGate>();
    gate->setParameter( "threshold", -55.0f );

    harness.graph().addNode( "gate", nodeOf( std::move( gate ) ) );
    harness.graph().addNode( "drive", overdriveNode( 25.0f ) );
    harness.graph().setSerialChain( { "gate", "drive" } );

    const std::vector<float> input =
        SignalGenerator::pluckedString( 110.0f, 48000, 48000, 0.6f );
    const std::vector<float> output = harness.leftChannel( harness.render( input ) );

    EXPECT_LE( SignalGenerator::peak( output ), 1.0f );
    EXPECT_GT( SignalGenerator::rms( output ), 0.0f );
}
