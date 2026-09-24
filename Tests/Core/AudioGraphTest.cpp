#include <gtest/gtest.h>

#include <memory>
#include <vector>

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
