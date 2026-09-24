#include <gtest/gtest.h>

#include <Core/Analysis/NoteHelper.h>
#include <Tests/Support/AudioHarness.h>
#include <Tests/Support/SignalGenerator.h>

using TestSupport::AudioHarness;
using TestSupport::SignalGenerator;

namespace {

constexpr uint32_t FEED_SAMPLES = 4096;

AudioHarness::Reading readPitch( float frequency ) {
    AudioHarness harness;
    harness.render( SignalGenerator::pluckedString( frequency, 48000, FEED_SAMPLES ) );
    return harness.waitForPitch();
}

} // namespace

TEST( TunerIntegrationTest, ReadsAPluckedStringThroughTheWholePipeline ) {
    const AudioHarness::Reading reading = readPitch( 110.0f );

    ASSERT_TRUE( reading.voiced );
    EXPECT_NEAR( reading.frequency, 110.0f, 2.0f );
    EXPECT_EQ( reading.midiNote, 45 );
    EXPECT_STREQ( Core::NoteHelper::name( reading.midiNote ), "A" );
}

TEST( TunerIntegrationTest, ReadsEveryOpenStringOfStandardTuning ) {
    const char* const expected[ 6 ] = { "E", "A", "D", "G", "B", "E" };

    for ( int string = 0; string < 6; string++ ) {
        const float target = SignalGenerator::STANDARD_TUNING[ string ];
        const AudioHarness::Reading reading = readPitch( target );

        ASSERT_TRUE( reading.voiced ) << "corda " << string << " nao foi detectada";
        EXPECT_NEAR( reading.frequency, target, target * 0.02f ) << "corda " << string;
        EXPECT_STREQ( Core::NoteHelper::name( reading.midiNote ), expected[ string ] ) << "corda " << string;
    }
}

TEST( TunerIntegrationTest, ReportsSharpAndFlatDeviation ) {
    const AudioHarness::Reading sharp = readPitch( 224.0f );
    const AudioHarness::Reading flat = readPitch( 216.0f );

    ASSERT_TRUE( sharp.voiced );
    ASSERT_TRUE( flat.voiced );
    EXPECT_GT( sharp.cents, 10.0f );
    EXPECT_LT( flat.cents, -10.0f );
}

TEST( TunerIntegrationTest, DcOffsetDoesNotBreakDetection ) {
    AudioHarness harness;

    std::vector<float> samples = SignalGenerator::pluckedString( 146.83f, 48000, FEED_SAMPLES );
    SignalGenerator::addDcOffset( samples, 0.2f );
    harness.render( samples );

    const AudioHarness::Reading reading = harness.waitForPitch();

    ASSERT_TRUE( reading.voiced );
    EXPECT_NEAR( reading.frequency, 146.83f, 3.0f );
}

TEST( TunerIntegrationTest, SilenceNeverReportsANote ) {
    AudioHarness harness;
    harness.render( SignalGenerator::silence( FEED_SAMPLES ) );

    EXPECT_FALSE( harness.waitForPitch( 300 ).voiced );
}

TEST( TunerIntegrationTest, NoiseIsNotMistakenForANote ) {
    AudioHarness harness;
    harness.render( SignalGenerator::whiteNoise( FEED_SAMPLES, 42, 0.3f ) );

    EXPECT_FALSE( harness.waitForPitch( 300 ).voiced );
}

TEST( TunerIntegrationTest, DisabledTunerStopsPublishingReadings ) {
    AudioHarness harness;
    harness.worker().setTunerEnabled( false );
    harness.render( SignalGenerator::pluckedString( 196.0f, 48000, FEED_SAMPLES ) );

    EXPECT_FALSE( harness.waitForPitch( 300 ).voiced );
}
