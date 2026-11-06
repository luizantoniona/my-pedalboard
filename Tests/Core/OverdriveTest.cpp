#include <algorithm>
#include <cmath>
#include <vector>

#include <gtest/gtest.h>

#include <Core/DSP/Effect/Distortion/Overdrive.h>
#include <Tests/Support/SignalGenerator.h>

namespace {

constexpr uint32_t SAMPLE_RATE = 48000;
constexpr uint32_t MAX_FRAMES = 128;

void pushBlocks( Core::Overdrive& overdrive, std::vector<float>& signal ) {
    for ( std::size_t offset = 0; offset < signal.size(); offset += MAX_FRAMES ) {
        const uint32_t frames =
            static_cast<uint32_t>( std::min<std::size_t>( MAX_FRAMES, signal.size() - offset ) );
        overdrive.process( signal.data() + offset, nullptr, frames );
    }
}

float crestFactor( const std::vector<float>& samples ) {
    const float rms = TestSupport::SignalGenerator::rms( samples );
    if ( rms <= 0.0f ) {
        return 0.0f;
    }
    return TestSupport::SignalGenerator::peak( samples ) / rms;
}

} // namespace

TEST( OverdriveTest, SilenceStaysSilent ) {
    Core::Overdrive overdrive;
    overdrive.prepare( SAMPLE_RATE, MAX_FRAMES );

    std::vector<float> block = TestSupport::SignalGenerator::silence( MAX_FRAMES );
    overdrive.process( block.data(), nullptr, MAX_FRAMES );

    EXPECT_FLOAT_EQ( TestSupport::SignalGenerator::peak( block ), 0.0f );
}

TEST( OverdriveTest, ShaperNeverLeavesUnitRangeAtUnityLevel ) {
    Core::Overdrive overdrive;
    overdrive.prepare( SAMPLE_RATE, MAX_FRAMES );
    overdrive.setParameter( "drive", 20.0f );
    overdrive.setParameter( "level", 0.0f );
    overdrive.setParameter( "mix", 1.0f );
    overdrive.reset();

    std::vector<float> signal =
        TestSupport::SignalGenerator::pluckedString( 110.0f, SAMPLE_RATE, SAMPLE_RATE / 4, 0.9f );
    pushBlocks( overdrive, signal );

    EXPECT_LE( TestSupport::SignalGenerator::peak( signal ), 1.0f );
}

TEST( OverdriveTest, DriveSquaresOffTheWaveform ) {
    const std::vector<float> source =
        TestSupport::SignalGenerator::pluckedString( 110.0f, SAMPLE_RATE, SAMPLE_RATE / 4, 0.5f );

    const auto crestWithDrive = [ & ]( float drive ) {
        Core::Overdrive overdrive;
        overdrive.prepare( SAMPLE_RATE, MAX_FRAMES );
        overdrive.setParameter( "drive", drive );
        overdrive.setParameter( "tone", 1.0f );
        overdrive.setParameter( "level", 0.0f );
        overdrive.setParameter( "mix", 1.0f );
        overdrive.reset();

        std::vector<float> signal = source;
        pushBlocks( overdrive, signal );
        return crestFactor( signal );
    };

    EXPECT_LT( crestWithDrive( 20.0f ), crestWithDrive( 1.0f ) );
}

TEST( OverdriveTest, DryMixLeavesTheSignalAlone ) {
    Core::Overdrive overdrive;
    overdrive.prepare( SAMPLE_RATE, MAX_FRAMES );
    overdrive.setParameter( "drive", 30.0f );
    overdrive.setParameter( "mix", 0.0f );
    overdrive.reset();

    const std::vector<float> source =
        TestSupport::SignalGenerator::sine( 220.0f, SAMPLE_RATE, SAMPLE_RATE / 10, 0.4f );
    std::vector<float> signal = source;
    pushBlocks( overdrive, signal );

    for ( std::size_t i = 0; i < source.size(); i++ ) {
        ASSERT_NEAR( signal[ i ], source[ i ], 1.0e-6f ) << "at sample " << i;
    }
}

TEST( OverdriveTest, ToneDownDarkensTheOutput ) {
    const std::vector<float> source =
        TestSupport::SignalGenerator::pluckedString( 110.0f, SAMPLE_RATE, SAMPLE_RATE / 4, 0.5f );

    const auto brightnessWithTone = [ & ]( float tone ) {
        Core::Overdrive overdrive;
        overdrive.prepare( SAMPLE_RATE, MAX_FRAMES );
        overdrive.setParameter( "drive", 20.0f );
        overdrive.setParameter( "tone", tone );
        overdrive.setParameter( "mix", 1.0f );
        overdrive.reset();

        std::vector<float> signal = source;
        pushBlocks( overdrive, signal );

        std::vector<float> difference( signal.size(), 0.0f );
        for ( std::size_t i = 1; i < signal.size(); i++ ) {
            difference[ i ] = signal[ i ] - signal[ i - 1 ];
        }
        return TestSupport::SignalGenerator::rms( difference );
    };

    EXPECT_LT( brightnessWithTone( 0.0f ), brightnessWithTone( 1.0f ) );
}

TEST( OverdriveTest, AsymmetricClippingLeavesNoDcOffset ) {
    Core::Overdrive overdrive;
    overdrive.prepare( SAMPLE_RATE, MAX_FRAMES );
    overdrive.setParameter( "drive", 20.0f );
    overdrive.setParameter( "level", 0.0f );
    overdrive.reset();

    std::vector<float> signal = TestSupport::SignalGenerator::sine( 220.0f, SAMPLE_RATE, SAMPLE_RATE, 0.5f );
    pushBlocks( overdrive, signal );

    double sum = 0.0;
    for ( std::size_t i = signal.size() / 2; i < signal.size(); i++ ) {
        sum += signal[ i ];
    }
    EXPECT_NEAR( sum / ( signal.size() / 2 ), 0.0, 0.02 );
}

TEST( OverdriveTest, BassIsTrimmedRelativeToTheMids ) {
    const auto rmsAt = [ & ]( float frequency ) {
        Core::Overdrive overdrive;
        overdrive.prepare( SAMPLE_RATE, MAX_FRAMES );
        overdrive.setParameter( "drive", 1.0f );
        overdrive.setParameter( "tone", 1.0f );
        overdrive.setParameter( "level", 0.0f );
        overdrive.reset();

        std::vector<float> signal = TestSupport::SignalGenerator::sine( frequency, SAMPLE_RATE, SAMPLE_RATE / 2, 0.05f );
        pushBlocks( overdrive, signal );
        return TestSupport::SignalGenerator::rms( std::vector<float>( signal.begin() + signal.size() / 2, signal.end() ) );
    };

    EXPECT_LT( rmsAt( 50.0f ), rmsAt( 720.0f ) * 0.5f );
}

TEST( OverdriveTest, LeavesSamplesPastTheBlockUntouched ) {
    Core::Overdrive overdrive;
    overdrive.prepare( SAMPLE_RATE, MAX_FRAMES );

    std::vector<float> block = TestSupport::SignalGenerator::constant( 0.5f, MAX_FRAMES );
    overdrive.process( block.data(), nullptr, 7 );

    EXPECT_FLOAT_EQ( block[ 7 ], 0.5f );
}

TEST( OverdriveTest, ClampsParametersAndReportsDescriptors ) {
    Core::Overdrive overdrive;

    EXPECT_EQ( overdrive.parameters().size(), 4u );
    EXPECT_EQ( overdrive.parameters()[ 0 ].name, "drive" );

    overdrive.setParameter( "drive", 500.0f );
    EXPECT_NEAR( overdrive.parameter( "drive" ), 20.0f, 0.001f );

    overdrive.setParameter( "mix", -3.0f );
    EXPECT_NEAR( overdrive.parameter( "mix" ), 0.0f, 0.001f );
}
