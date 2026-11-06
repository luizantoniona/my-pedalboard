#include <algorithm>
#include <cmath>
#include <vector>

#include <gtest/gtest.h>

#include <Core/DSP/Effect/Dynamics/NoiseGate.h>
#include <Tests/Support/SignalGenerator.h>

namespace {

constexpr uint32_t SAMPLE_RATE = 48000;
constexpr uint32_t MAX_FRAMES = 128;

void pushBlocks( Core::NoiseGate& gate, std::vector<float>& signal ) {
    for ( std::size_t offset = 0; offset < signal.size(); offset += MAX_FRAMES ) {
        const uint32_t frames =
            static_cast<uint32_t>( std::min<std::size_t>( MAX_FRAMES, signal.size() - offset ) );
        gate.process( signal.data() + offset, nullptr, frames );
    }
}

std::vector<float> tailOf( const std::vector<float>& samples, uint32_t count ) {
    const std::size_t taken = std::min<std::size_t>( count, samples.size() );
    return std::vector<float>( samples.end() - static_cast<long>( taken ), samples.end() );
}

} // namespace

TEST( NoiseGateTest, SilenceStaysSilent ) {
    Core::NoiseGate gate;
    gate.prepare( SAMPLE_RATE, MAX_FRAMES );

    std::vector<float> block = TestSupport::SignalGenerator::silence( MAX_FRAMES );
    gate.process( block.data(), nullptr, MAX_FRAMES );

    EXPECT_FLOAT_EQ( TestSupport::SignalGenerator::peak( block ), 0.0f );
}

TEST( NoiseGateTest, PassesSignalAboveThreshold ) {
    Core::NoiseGate gate;
    gate.prepare( SAMPLE_RATE, MAX_FRAMES );
    gate.setParameter( "threshold", -40.0f );
    gate.setParameter( "attack", 1.0f );

    std::vector<float> signal =
        TestSupport::SignalGenerator::sine( 220.0f, SAMPLE_RATE, SAMPLE_RATE / 4, 0.5f );
    pushBlocks( gate, signal );

    EXPECT_NEAR( TestSupport::SignalGenerator::peak( tailOf( signal, SAMPLE_RATE / 10 ) ), 0.5f, 0.02f );
}

TEST( NoiseGateTest, ClosesOnNoiseBelowThreshold ) {
    Core::NoiseGate gate;
    gate.prepare( SAMPLE_RATE, MAX_FRAMES );
    gate.setParameter( "threshold", -40.0f );
    gate.setParameter( "release", 20.0f );
    gate.setParameter( "hold", 0.0f );

    std::vector<float> hiss =
        TestSupport::SignalGenerator::whiteNoise( SAMPLE_RATE / 2, 1234, 0.002f );
    pushBlocks( gate, hiss );

    EXPECT_LT( TestSupport::SignalGenerator::peak( tailOf( hiss, SAMPLE_RATE / 10 ) ), 1.0e-5f );
}

TEST( NoiseGateTest, HoldKeepsGateOpenAfterTheNoteDrops ) {
    const std::vector<float> loud =
        TestSupport::SignalGenerator::sine( 220.0f, SAMPLE_RATE, SAMPLE_RATE / 10, 0.5f );
    const std::vector<float> quiet =
        TestSupport::SignalGenerator::sine( 220.0f, SAMPLE_RATE, ( SAMPLE_RATE * 3 ) / 10, 0.002f );

    const auto runWithHold = [ & ]( float holdMs ) {
        Core::NoiseGate gate;
        gate.prepare( SAMPLE_RATE, MAX_FRAMES );
        gate.setParameter( "threshold", -40.0f );
        gate.setParameter( "attack", 0.5f );
        gate.setParameter( "release", 5.0f );
        gate.setParameter( "hold", holdMs );

        std::vector<float> signal = loud;
        signal.insert( signal.end(), quiet.begin(), quiet.end() );
        pushBlocks( gate, signal );

        return TestSupport::SignalGenerator::peak( tailOf( signal, SAMPLE_RATE / 40 ) );
    };

    EXPECT_NEAR( runWithHold( 500.0f ), 0.002f, 0.0002f );
    EXPECT_LT( runWithHold( 0.0f ), 1.0e-5f );
}

TEST( NoiseGateTest, LeavesSamplesPastTheBlockUntouched ) {
    Core::NoiseGate gate;
    gate.prepare( SAMPLE_RATE, MAX_FRAMES );

    std::vector<float> block = TestSupport::SignalGenerator::constant( 0.5f, MAX_FRAMES );
    gate.process( block.data(), nullptr, 7 );

    EXPECT_FLOAT_EQ( block[ 7 ], 0.5f );
}

TEST( NoiseGateTest, ClampsParametersAndReportsDescriptors ) {
    Core::NoiseGate gate;

    EXPECT_EQ( gate.parameters().size(), 4u );
    EXPECT_EQ( gate.parameters()[ 0 ].name, "threshold" );

    gate.setParameter( "threshold", -35.0f );
    EXPECT_NEAR( gate.parameter( "threshold" ), -35.0f, 0.01f );

    gate.setParameter( "attack", 9999.0f );
    EXPECT_NEAR( gate.parameter( "attack" ), 50.0f, 0.001f );
}
