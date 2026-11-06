#include <algorithm>
#include <cmath>
#include <vector>

#include <gtest/gtest.h>

#include <Core/DSP/Effect/Delay/Delay.h>
#include <Tests/Support/SignalGenerator.h>

namespace {

constexpr uint32_t SAMPLE_RATE = 48000;
constexpr uint32_t MAX_FRAMES = 128;

void pushBlocks( Core::Delay& delay, std::vector<float>& signal ) {
    for ( std::size_t offset = 0; offset < signal.size(); offset += MAX_FRAMES ) {
        const uint32_t frames =
            static_cast<uint32_t>( std::min<std::size_t>( MAX_FRAMES, signal.size() - offset ) );
        delay.process( signal.data() + offset, nullptr, frames );
    }
}

void configure( Core::Delay& delay, float timeMilliseconds, float feedback, float mix ) {
    delay.prepare( SAMPLE_RATE, MAX_FRAMES );
    delay.setParameter( "time", timeMilliseconds );
    delay.setParameter( "feedback", feedback );
    delay.setParameter( "tone", 1.0f );
    delay.setParameter( "mix", mix );
    delay.reset();
}

float energyBetween( const std::vector<float>& signal, std::size_t from, std::size_t to ) {
    float energy = 0.0f;
    for ( std::size_t i = from; i < to && i < signal.size(); i++ ) {
        energy += signal[ i ] * signal[ i ];
    }
    return energy;
}

} // namespace

TEST( DelayTest, SilenceStaysSilent ) {
    Core::Delay delay;
    configure( delay, 100.0f, 0.5f, 1.0f );

    std::vector<float> block = TestSupport::SignalGenerator::silence( MAX_FRAMES );
    delay.process( block.data(), nullptr, MAX_FRAMES );

    EXPECT_FLOAT_EQ( TestSupport::SignalGenerator::peak( block ), 0.0f );
}

TEST( DelayTest, EchoArrivesAfterTheSetTime ) {
    Core::Delay delay;
    configure( delay, 100.0f, 0.0f, 1.0f );
    const std::size_t echoIndex = 4800;

    std::vector<float> signal = TestSupport::SignalGenerator::impulse( 12000 );
    pushBlocks( delay, signal );

    EXPECT_NEAR( signal[ 0 ], 1.0f, 1.0e-6f );
    EXPECT_NEAR( signal[ echoIndex ], 1.0f, 1.0e-4f );
    EXPECT_FLOAT_EQ( energyBetween( signal, 1, echoIndex ), 0.0f );
    EXPECT_FLOAT_EQ( energyBetween( signal, echoIndex + 1, signal.size() ), 0.0f );
}

TEST( DelayTest, FeedbackRepeatsTheEchoAndDecaysIt ) {
    Core::Delay delay;
    configure( delay, 100.0f, 0.5f, 1.0f );

    std::vector<float> signal = TestSupport::SignalGenerator::impulse( 20000 );
    pushBlocks( delay, signal );

    const float firstEcho = energyBetween( signal, 4700, 5000 );
    const float secondEcho = energyBetween( signal, 9500, 9800 );
    const float thirdEcho = energyBetween( signal, 14300, 14600 );

    EXPECT_GT( secondEcho, 0.0f );
    EXPECT_LT( secondEcho, firstEcho );
    EXPECT_LT( thirdEcho, secondEcho );
}

TEST( DelayTest, ZeroMixLeavesTheSignalAlone ) {
    Core::Delay delay;
    configure( delay, 100.0f, 0.8f, 0.0f );

    const std::vector<float> source = TestSupport::SignalGenerator::sine( 220.0f, SAMPLE_RATE, SAMPLE_RATE / 4, 0.4f );
    std::vector<float> signal = source;
    pushBlocks( delay, signal );

    for ( std::size_t i = 0; i < source.size(); i++ ) {
        ASSERT_NEAR( signal[ i ], source[ i ], 1.0e-6f ) << "at sample " << i;
    }
}

TEST( DelayTest, MaximumFeedbackStaysBounded ) {
    Core::Delay delay;
    configure( delay, 30.0f, 10.0f, 1.0f );

    std::vector<float> signal = TestSupport::SignalGenerator::whiteNoise( SAMPLE_RATE * 2, 3, 0.3f );
    pushBlocks( delay, signal );

    EXPECT_TRUE( std::isfinite( TestSupport::SignalGenerator::peak( signal ) ) );
    EXPECT_LT( TestSupport::SignalGenerator::peak( signal ), 20.0f );
}

TEST( DelayTest, MovingTheTimeKnobStaysContinuous ) {
    Core::Delay delay;
    configure( delay, 100.0f, 0.3f, 0.5f );

    std::vector<float> signal = TestSupport::SignalGenerator::sine( 220.0f, SAMPLE_RATE, SAMPLE_RATE, 0.3f );
    for ( std::size_t offset = 0; offset < signal.size(); offset += MAX_FRAMES ) {
        if ( offset == SAMPLE_RATE / 2 ) {
            delay.setParameter( "time", 700.0f );
        }
        const uint32_t frames =
            static_cast<uint32_t>( std::min<std::size_t>( MAX_FRAMES, signal.size() - offset ) );
        delay.process( signal.data() + offset, nullptr, frames );
    }

    float largestStep = 0.0f;
    for ( std::size_t i = 1; i < signal.size(); i++ ) {
        largestStep = std::max( largestStep, std::fabs( signal[ i ] - signal[ i - 1 ] ) );
    }
    EXPECT_LT( largestStep, 0.2f );
}

TEST( DelayTest, ProcessBeforePrepareDoesNothing ) {
    Core::Delay delay;

    std::vector<float> block = TestSupport::SignalGenerator::constant( 0.5f, MAX_FRAMES );
    delay.process( block.data(), nullptr, MAX_FRAMES );

    EXPECT_FLOAT_EQ( block[ 0 ], 0.5f );
}

TEST( DelayTest, LeavesSamplesPastTheBlockUntouched ) {
    Core::Delay delay;
    configure( delay, 100.0f, 0.5f, 1.0f );

    std::vector<float> block = TestSupport::SignalGenerator::constant( 0.5f, MAX_FRAMES );
    delay.process( block.data(), nullptr, 7 );

    EXPECT_FLOAT_EQ( block[ 7 ], 0.5f );
}

TEST( DelayTest, ClampsParametersAndReportsDescriptors ) {
    Core::Delay delay;

    EXPECT_EQ( delay.parameters().size(), 4u );
    EXPECT_EQ( delay.parameters()[ 0 ].name, "time" );
    EXPECT_EQ( delay.typeName(), "Delay" );

    delay.setParameter( "time", 99999.0f );
    EXPECT_NEAR( delay.parameter( "time" ), 1500.0f, 0.001f );

    delay.setParameter( "feedback", 5.0f );
    EXPECT_NEAR( delay.parameter( "feedback" ), 0.95f, 0.001f );
}
