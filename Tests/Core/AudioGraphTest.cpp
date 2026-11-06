#include <gtest/gtest.h>

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include <Core/DSP/Effect/Distortion/Overdrive.h>
#include <Core/DSP/Effect/Utility/Gain.h>
#include <Core/DSP/EffectNode.h>
#include <Core/Graph/AudioGraph.h>

namespace {

std::shared_ptr<Core::EffectNode> makeGain( float decibels ) {
    auto gain = std::make_unique<Core::Gain>();
    gain->setParameter( "gainDb", decibels );
    return std::make_shared<Core::EffectNode>( std::move( gain ) );
}

} // namespace

TEST( AudioGraphTest, EmptyGraphLeavesBufferUntouched ) {
    Core::AudioGraph graph;
    graph.prepare( 48000, 512 );

    std::vector<float> samples( 512, 0.25f );
    Core::AudioBuffer buffer( samples.data(), nullptr, 512, 48000 );
    graph.process( buffer );

    EXPECT_FLOAT_EQ( samples.front(), 0.25f );
}

TEST( AudioGraphTest, ProcessesNodesInTopologicalOrder ) {
    Core::AudioGraph graph;
    graph.prepare( 48000, 512 );

    graph.addNode( "a", makeGain( -6.0f ) );
    graph.addNode( "b", makeGain( -6.0f ) );
    graph.connect( "a", "b" );

    std::vector<float> samples( 512, 1.0f );
    Core::AudioBuffer buffer( samples.data(), nullptr, 512, 48000 );

    for ( int block = 0; block < 60; block++ ) {
        std::fill( samples.begin(), samples.end(), 1.0f );
        graph.process( buffer );
    }

    EXPECT_NEAR( samples.back(), 0.2511f, 1.0e-3f );
}

TEST( AudioGraphTest, RemovingNodeDropsItsEdges ) {
    Core::AudioGraph graph;
    graph.prepare( 48000, 512 );

    graph.addNode( "a", makeGain( 0.0f ) );
    graph.addNode( "b", makeGain( 0.0f ) );
    graph.connect( "a", "b" );
    graph.removeNode( "b" );

    EXPECT_EQ( graph.nodes().size(), 1u );
    EXPECT_TRUE( graph.edges().empty() );
}

TEST( AudioGraphTest, SurvivesRebuildWhileProcessing ) {
    Core::AudioGraph graph;
    graph.prepare( 48000, 256 );

    std::vector<float> samples( 256, 0.5f );
    Core::AudioBuffer buffer( samples.data(), nullptr, 256, 48000 );

    for ( int i = 0; i < 40; i++ ) {
        graph.addNode( "node-" + std::to_string( i ), makeGain( 0.0f ) );
        graph.process( buffer );
        graph.process( buffer );
    }

    EXPECT_EQ( graph.nodes().size(), 40u );
}

TEST( AudioGraphTest, DisabledEffectIsSkipped ) {
    Core::AudioGraph graph;
    graph.prepare( 48000, 512 );

    auto silencer = std::make_unique<Core::Gain>();
    silencer->setParameter( "gainDb", -60.0f );
    silencer->setEnabled( false );
    graph.addNode( "muted", std::make_shared<Core::EffectNode>( std::move( silencer ) ) );

    std::vector<float> samples( 512, 1.0f );
    Core::AudioBuffer buffer( samples.data(), nullptr, 512, 48000 );
    graph.process( buffer );

    EXPECT_FLOAT_EQ( samples.front(), 1.0f );
}

TEST( AudioGraphTest, SerialChainReplacesThePreviousEdges ) {
    Core::AudioGraph graph;
    graph.prepare( 48000, 512 );

    graph.addNode( "a", makeGain( 0.0f ) );
    graph.addNode( "b", makeGain( 0.0f ) );
    graph.addNode( "c", makeGain( 0.0f ) );

    graph.setSerialChain( { "a", "b", "c" } );
    ASSERT_EQ( graph.edges().size(), 2u );

    graph.setSerialChain( { "c", "a" } );

    ASSERT_EQ( graph.edges().size(), 1u );
    EXPECT_EQ( graph.edges().front().from, "c" );
    EXPECT_EQ( graph.edges().front().to, "a" );
}

TEST( AudioGraphTest, SerialChainSkipsIdsWithoutANode ) {
    Core::AudioGraph graph;
    graph.prepare( 48000, 512 );

    graph.addNode( "a", makeGain( 0.0f ) );
    graph.addNode( "c", makeGain( 0.0f ) );

    graph.setSerialChain( { "a", "ghost", "c" } );

    ASSERT_EQ( graph.edges().size(), 1u );
    EXPECT_EQ( graph.edges().front().from, "a" );
    EXPECT_EQ( graph.edges().front().to, "c" );
}

TEST( AudioGraphTest, ChainOrderChangesTheNonlinearResult ) {
    const auto runChain = []( const std::vector<std::string>& order ) {
        Core::AudioGraph graph;
        graph.prepare( 48000, 512 );

        auto overdrive = std::make_unique<Core::Overdrive>();
        overdrive->setParameter( "drive", 20.0f );
        overdrive->setParameter( "level", 0.0f );
        overdrive->setParameter( "mix", 1.0f );

        graph.addNode( "boost", makeGain( 18.0f ) );
        graph.addNode( "drive", std::make_shared<Core::EffectNode>( std::move( overdrive ) ) );
        graph.setSerialChain( order );

        std::vector<float> samples( 512, 0.0f );
        Core::AudioBuffer buffer( samples.data(), nullptr, 512, 48000 );

        float peak = 0.0f;
        for ( int block = 0; block < 60; block++ ) {
            for ( int i = 0; i < 512; i++ ) {
                samples[ i ] = 0.05f * std::sin( 6.2831853f * 440.0f * static_cast<float>( block * 512 + i ) / 48000.0f );
            }
            graph.process( buffer );
            peak = 0.0f;
            for ( const float sample : samples ) {
                peak = std::max( peak, std::fabs( sample ) );
            }
        }
        return peak;
    };

    EXPECT_GT( std::fabs( runChain( { "boost", "drive" } ) - runChain( { "drive", "boost" } ) ), 0.01f );
}
