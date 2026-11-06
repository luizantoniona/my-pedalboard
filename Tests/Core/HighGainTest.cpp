#include <algorithm>
#include <cmath>
#include <vector>

#include <gtest/gtest.h>

#include <Core/DSP/Effect/Distortion/HighGain.h>
#include <Tests/Support/SignalGenerator.h>

namespace {

constexpr uint32_t SAMPLE_RATE = 48000;
constexpr uint32_t MAX_FRAMES = 128;

void pushBlocks( Core::HighGain& pedal, std::vector<float>& signal ) {
    for ( std::size_t offset = 0; offset < signal.size(); offset += MAX_FRAMES ) {
        const uint32_t frames =
            static_cast<uint32_t>( std::min<std::size_t>( MAX_FRAMES, signal.size() - offset ) );
        pedal.process( signal.data() + offset, nullptr, frames );
    }
}

void configure( Core::HighGain& pedal, float distortion, float level ) {
    pedal.prepare( SAMPLE_RATE, MAX_FRAMES );
    pedal.setParameter( "distortion", distortion );
    pedal.setParameter( "level", level );
    pedal.reset();
}

float settledRms( const std::vector<float>& signal ) {
    return TestSupport::SignalGenerator::rms( std::vector<float>( signal.begin() + signal.size() / 2, signal.end() ) );
}

float crestFactor( const std::vector<float>& samples ) {
    const float rms = TestSupport::SignalGenerator::rms( samples );
    return rms > 0.0f ? TestSupport::SignalGenerator::peak( samples ) / rms : 0.0f;
}

} // namespace

TEST( HighGainTest, SilenceStaysSilent ) {
    Core::HighGain pedal;
    configure( pedal, 1.0f, 0.0f );

    std::vector<float> block = TestSupport::SignalGenerator::silence( MAX_FRAMES );
    pedal.process( block.data(), nullptr, MAX_FRAMES );

    EXPECT_FLOAT_EQ( TestSupport::SignalGenerator::peak( block ), 0.0f );
}

TEST( HighGainTest, FlatToneNeverLeavesUnitRange ) {
    Core::HighGain pedal;
    configure( pedal, 1.0f, 0.0f );

    std::vector<float> signal = TestSupport::SignalGenerator::pluckedString( 110.0f, SAMPLE_RATE, SAMPLE_RATE / 2, 0.9f );
    pushBlocks( pedal, signal );

    EXPECT_LE( TestSupport::SignalGenerator::peak( signal ), 1.0f );
}

TEST( HighGainTest, MoreDistortionSquaresOffTheWaveform ) {
    const std::vector<float> source = TestSupport::SignalGenerator::sine( 220.0f, SAMPLE_RATE, SAMPLE_RATE / 2, 0.3f );

    const auto crestWith = [ & ]( float distortion ) {
        Core::HighGain pedal;
        configure( pedal, distortion, 0.0f );
        std::vector<float> signal = source;
        pushBlocks( pedal, signal );
        return crestFactor( std::vector<float>( signal.begin() + signal.size() / 2, signal.end() ) );
    };

    EXPECT_LT( crestWith( 1.0f ), crestWith( 0.0f ) );
    EXPECT_LT( crestWith( 1.0f ), 1.25f );
}

TEST( HighGainTest, OutputStaysLoudWhenTheInputGetsQuiet ) {
    const auto rmsFor = [ & ]( float amplitude ) {
        Core::HighGain pedal;
        configure( pedal, 1.0f, 0.0f );
        std::vector<float> signal = TestSupport::SignalGenerator::sine( 220.0f, SAMPLE_RATE, SAMPLE_RATE / 2, amplitude );
        pushBlocks( pedal, signal );
        return settledRms( signal );
    };

    EXPECT_GT( rmsFor( 0.05f ), 0.7f * rmsFor( 0.5f ) );
}

TEST( HighGainTest, LowControlBoostsAndCutsTheBass ) {
    const auto bassRms = [ & ]( float lowDb ) {
        Core::HighGain pedal;
        configure( pedal, 0.3f, 0.0f );
        pedal.setParameter( "low", lowDb );
        pedal.reset();
        std::vector<float> signal = TestSupport::SignalGenerator::sine( 100.0f, SAMPLE_RATE, SAMPLE_RATE, 0.05f );
        pushBlocks( pedal, signal );
        return settledRms( signal );
    };

    EXPECT_GT( bassRms( 12.0f ), bassRms( -12.0f ) * 2.0f );
}

TEST( HighGainTest, MidControlActsAtTheSweptFrequency ) {
    const auto rmsWith = [ & ]( float midDb, float midFrequency, float tone ) {
        Core::HighGain pedal;
        configure( pedal, 0.0f, 0.0f );
        pedal.setParameter( "mid", midDb );
        pedal.setParameter( "midFrequency", midFrequency );
        pedal.reset();
        std::vector<float> signal = TestSupport::SignalGenerator::sine( tone, SAMPLE_RATE, SAMPLE_RATE, 0.02f );
        pushBlocks( pedal, signal );
        return settledRms( signal );
    };

    EXPECT_GT( rmsWith( 12.0f, 2000.0f, 2000.0f ), rmsWith( 0.0f, 2000.0f, 2000.0f ) * 2.0f );
    EXPECT_LT( rmsWith( 12.0f, 400.0f, 2000.0f ), rmsWith( 12.0f, 2000.0f, 2000.0f ) );
}

TEST( HighGainTest, SubBassIsTightenedBeforeClipping ) {
    const auto rmsAt = [ & ]( float frequency ) {
        Core::HighGain pedal;
        configure( pedal, 0.2f, 0.0f );
        std::vector<float> signal = TestSupport::SignalGenerator::sine( frequency, SAMPLE_RATE, SAMPLE_RATE, 0.02f );
        pushBlocks( pedal, signal );
        return settledRms( signal );
    };

    EXPECT_LT( rmsAt( 30.0f ), rmsAt( 400.0f ) * 0.5f );
}

TEST( HighGainTest, StereoChannelsGetTheSameTreatment ) {
    Core::HighGain pedal;
    configure( pedal, 0.8f, -6.0f );
    pedal.setParameter( "mid", 6.0f );

    std::vector<float> left = TestSupport::SignalGenerator::sine( 330.0f, SAMPLE_RATE, MAX_FRAMES, 0.3f );
    std::vector<float> right = left;
    pedal.process( left.data(), right.data(), MAX_FRAMES );

    for ( uint32_t i = 0; i < MAX_FRAMES; i++ ) {
        ASSERT_FLOAT_EQ( left[ i ], right[ i ] ) << "at sample " << i;
    }
}

TEST( HighGainTest, LeavesSamplesPastTheBlockUntouched ) {
    Core::HighGain pedal;
    configure( pedal, 1.0f, 0.0f );

    std::vector<float> block = TestSupport::SignalGenerator::constant( 0.5f, MAX_FRAMES );
    pedal.process( block.data(), nullptr, 7 );

    EXPECT_FLOAT_EQ( block[ 7 ], 0.5f );
}

TEST( HighGainTest, ClampsParametersAndReportsDescriptors ) {
    Core::HighGain pedal;

    EXPECT_EQ( pedal.parameters().size(), 6u );
    EXPECT_EQ( pedal.parameters()[ 0 ].name, "distortion" );
    EXPECT_EQ( pedal.typeName(), "HighGain" );

    pedal.setParameter( "distortion", 9.0f );
    EXPECT_NEAR( pedal.parameter( "distortion" ), 1.0f, 0.001f );

    pedal.setParameter( "low", 99.0f );
    EXPECT_NEAR( pedal.parameter( "low" ), 15.0f, 0.001f );

    pedal.setParameter( "midFrequency", 1.0f );
    EXPECT_NEAR( pedal.parameter( "midFrequency" ), 200.0f, 0.001f );
}
