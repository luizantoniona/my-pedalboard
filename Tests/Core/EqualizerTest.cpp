#include <algorithm>
#include <cmath>
#include <vector>

#include <gtest/gtest.h>

#include <Core/DSP/Effect/Filter/Equalizer.h>
#include <Tests/Support/SignalGenerator.h>

namespace {

constexpr uint32_t SAMPLE_RATE = 48000;
constexpr uint32_t MAX_FRAMES = 128;

void pushBlocks( Core::Equalizer& eq, std::vector<float>& signal ) {
    for ( std::size_t offset = 0; offset < signal.size(); offset += MAX_FRAMES ) {
        const uint32_t frames =
            static_cast<uint32_t>( std::min<std::size_t>( MAX_FRAMES, signal.size() - offset ) );
        eq.process( signal.data() + offset, nullptr, frames );
    }
}

float gainDbAt( Core::Equalizer& eq, float frequency ) {
    eq.reset();

    const std::vector<float> source = TestSupport::SignalGenerator::sine( frequency, SAMPLE_RATE, SAMPLE_RATE / 2, 0.1f );
    std::vector<float> signal = source;
    pushBlocks( eq, signal );

    const std::size_t settled = signal.size() / 2;
    const std::vector<float> tailIn( source.begin() + settled, source.end() );
    const std::vector<float> tailOut( signal.begin() + settled, signal.end() );
    return 20.0f * std::log10( TestSupport::SignalGenerator::rms( tailOut ) / TestSupport::SignalGenerator::rms( tailIn ) );
}

} // namespace

TEST( EqualizerTest, IsNamedEqualizer ) {
    Core::Equalizer eq;

    EXPECT_EQ( eq.typeName(), "Equalizer" );
}

TEST( EqualizerTest, SilenceStaysSilent ) {
    Core::Equalizer eq;
    eq.prepare( SAMPLE_RATE, MAX_FRAMES );
    eq.setParameter( "band800", 12.0f );

    std::vector<float> block = TestSupport::SignalGenerator::silence( MAX_FRAMES );
    eq.process( block.data(), nullptr, MAX_FRAMES );

    EXPECT_FLOAT_EQ( TestSupport::SignalGenerator::peak( block ), 0.0f );
}

TEST( EqualizerTest, FlatSettingsPassTheSignalThrough ) {
    Core::Equalizer eq;
    eq.prepare( SAMPLE_RATE, MAX_FRAMES );

    for ( const float frequency : { 80.0f, 800.0f, 6000.0f } ) {
        EXPECT_NEAR( gainDbAt( eq, frequency ), 0.0f, 0.05f ) << frequency << " Hz";
    }
}

TEST( EqualizerTest, BandBoostPeaksAtItsCentreAndFadesAway ) {
    Core::Equalizer eq;
    eq.prepare( SAMPLE_RATE, MAX_FRAMES );
    eq.setParameter( "band800", 12.0f );

    EXPECT_NEAR( gainDbAt( eq, 800.0f ), 12.0f, 0.3f );
    EXPECT_GT( gainDbAt( eq, 600.0f ), 6.0f );
    EXPECT_LT( gainDbAt( eq, 100.0f ), 1.5f );
    EXPECT_LT( gainDbAt( eq, 6400.0f ), 1.5f );
}

TEST( EqualizerTest, BandCutAttenuatesAroundItsCentre ) {
    Core::Equalizer eq;
    eq.prepare( SAMPLE_RATE, MAX_FRAMES );
    eq.setParameter( "band3200", -15.0f );

    EXPECT_NEAR( gainDbAt( eq, 3200.0f ), -15.0f, 0.3f );
    EXPECT_NEAR( gainDbAt( eq, 200.0f ), 0.0f, 0.5f );
}

TEST( EqualizerTest, EveryBandActsOnItsOwnFrequency ) {
    const char* names[] = { "band100", "band200", "band400", "band800", "band1600", "band3200", "band6400" };
    const float frequencies[] = { 100.0f, 200.0f, 400.0f, 800.0f, 1600.0f, 3200.0f, 6400.0f };

    for ( int i = 0; i < 7; i++ ) {
        Core::Equalizer eq;
        eq.prepare( SAMPLE_RATE, MAX_FRAMES );
        eq.setParameter( names[ i ], 10.0f );

        EXPECT_NEAR( gainDbAt( eq, frequencies[ i ] ), 10.0f, 0.3f ) << names[ i ];
    }
}

TEST( EqualizerTest, LevelScalesTheWholeSignal ) {
    Core::Equalizer eq;
    eq.prepare( SAMPLE_RATE, MAX_FRAMES );
    eq.setParameter( "level", 6.0f );
    eq.reset();

    EXPECT_NEAR( gainDbAt( eq, 440.0f ), 6.0f, 0.1f );
    EXPECT_NEAR( eq.parameter( "level" ), 6.0f, 0.001f );
}

TEST( EqualizerTest, StereoChannelsGetTheSameTreatment ) {
    Core::Equalizer eq;
    eq.prepare( SAMPLE_RATE, MAX_FRAMES );
    eq.setParameter( "band800", 9.0f );
    eq.setParameter( "level", 3.0f );

    std::vector<float> left = TestSupport::SignalGenerator::sine( 800.0f, SAMPLE_RATE, MAX_FRAMES, 0.2f );
    std::vector<float> right = left;
    eq.process( left.data(), right.data(), MAX_FRAMES );

    for ( uint32_t i = 0; i < MAX_FRAMES; i++ ) {
        ASSERT_FLOAT_EQ( left[ i ], right[ i ] ) << "at sample " << i;
    }
}

TEST( EqualizerTest, LeavesSamplesPastTheBlockUntouched ) {
    Core::Equalizer eq;
    eq.prepare( SAMPLE_RATE, MAX_FRAMES );
    eq.setParameter( "band800", 12.0f );

    std::vector<float> block = TestSupport::SignalGenerator::constant( 0.5f, MAX_FRAMES );
    eq.process( block.data(), nullptr, 7 );

    EXPECT_FLOAT_EQ( block[ 7 ], 0.5f );
}

TEST( EqualizerTest, ClampsParametersAndReportsDescriptors ) {
    Core::Equalizer eq;

    EXPECT_EQ( eq.parameters().size(), 8u );
    EXPECT_EQ( eq.parameters()[ 0 ].name, "band100" );
    EXPECT_EQ( eq.parameters()[ 7 ].name, "level" );

    eq.setParameter( "band800", 99.0f );
    EXPECT_NEAR( eq.parameter( "band800" ), 15.0f, 0.001f );

    eq.setParameter( "level", -99.0f );
    EXPECT_NEAR( eq.parameter( "level" ), -15.0f, 0.01f );
}
