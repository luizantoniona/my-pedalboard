#include <gtest/gtest.h>

#include <vector>

#include <Core/Analysis/NoteHelper.h>
#include <Core/Analysis/PitchDetector.h>
#include <Tests/Support/SignalGenerator.h>

using TestSupport::SignalGenerator;

namespace {

Core::PitchDetector::Result analyze( const std::vector<float>& samples ) {
    Core::PitchDetector detector;
    detector.prepare( 48000 );
    return detector.analyze( samples.data(), static_cast<uint32_t>( samples.size() ) );
}

uint32_t windowSize() {
    Core::PitchDetector detector;
    detector.prepare( 48000 );
    return detector.windowSize();
}

} // namespace

TEST( PitchDetectorTest, DetectsLowE ) {
    const Core::PitchDetector::Result result = analyze( SignalGenerator::sine( 82.41f, 48000, windowSize() ) );

    ASSERT_TRUE( result.voiced );
    EXPECT_NEAR( result.frequency, 82.41f, 1.5f );
}

TEST( PitchDetectorTest, DetectsA440 ) {
    const Core::PitchDetector::Result result = analyze( SignalGenerator::sine( 440.0f, 48000, windowSize() ) );

    ASSERT_TRUE( result.voiced );
    EXPECT_NEAR( result.frequency, 440.0f, 6.0f );
}

// A harmonic-rich signal is where a detector typically locks onto the second
// partial and reports the note an octave high.
TEST( PitchDetectorTest, DoesNotJumpAnOctaveOnAHarmonicRichSignal ) {
    const Core::PitchDetector::Result result = analyze( SignalGenerator::pluckedString( 110.0f, 48000, windowSize() ) );

    ASSERT_TRUE( result.voiced );
    EXPECT_NEAR( result.frequency, 110.0f, 3.0f );
}

TEST( PitchDetectorTest, TracksEveryOpenStringOfStandardTuning ) {
    for ( float target : SignalGenerator::STANDARD_TUNING ) {
        const Core::PitchDetector::Result result = analyze( SignalGenerator::pluckedString( target, 48000, windowSize() ) );

        ASSERT_TRUE( result.voiced ) << target << " Hz nao detectado";
        EXPECT_NEAR( result.frequency, target, target * 0.02f );
    }
}

TEST( PitchDetectorTest, SilenceIsUnvoiced ) {
    EXPECT_FALSE( analyze( SignalGenerator::silence( windowSize() ) ).voiced );
}

TEST( PitchDetectorTest, NoiseIsUnvoiced ) {
    EXPECT_FALSE( analyze( SignalGenerator::whiteNoise( windowSize(), 3, 0.3f ) ).voiced );
}

TEST( PitchDetectorTest, VeryQuietSignalIsUnvoiced ) {
    EXPECT_FALSE( analyze( SignalGenerator::sine( 220.0f, 48000, windowSize(), 0.0005f ) ).voiced );
}

TEST( PitchDetectorTest, RejectsShortInput ) {
    EXPECT_FALSE( analyze( SignalGenerator::sine( 220.0f, 48000, 64 ) ).voiced );
}

TEST( PitchDetectorTest, ClarityIsHigherForAToneThanForNoise ) {
    const Core::PitchDetector::Result tone = analyze( SignalGenerator::sine( 196.0f, 48000, windowSize() ) );
    const Core::PitchDetector::Result noise = analyze( SignalGenerator::whiteNoise( windowSize(), 11, 0.5f ) );

    ASSERT_TRUE( tone.voiced );
    EXPECT_GT( tone.clarity, noise.clarity );
}

TEST( NoteHelperTest, A440IsMidi69WithoutDeviation ) {
    const Core::NoteInfo info = Core::NoteHelper::fromFrequency( 440.0f );

    EXPECT_EQ( info.midiNote, 69 );
    EXPECT_NEAR( info.cents, 0.0f, 0.01f );
    EXPECT_STREQ( Core::NoteHelper::name( info.midiNote ), "A" );
    EXPECT_EQ( Core::NoteHelper::octave( info.midiNote ), 4 );
}

TEST( NoteHelperTest, ReportsDeviationInCents ) {
    const Core::NoteInfo info = Core::NoteHelper::fromFrequency( 445.0f );

    EXPECT_EQ( info.midiNote, 69 );
    EXPECT_NEAR( info.cents, 19.56f, 0.5f );
}

TEST( NoteHelperTest, RoundTripsEveryOpenString ) {
    const char* const expected[ 6 ] = { "E", "A", "D", "G", "B", "E" };

    for ( int string = 0; string < 6; string++ ) {
        const Core::NoteInfo info = Core::NoteHelper::fromFrequency( SignalGenerator::STANDARD_TUNING[ string ] );

        EXPECT_STREQ( Core::NoteHelper::name( info.midiNote ), expected[ string ] );
        EXPECT_NEAR( Core::NoteHelper::frequency( info.midiNote ), SignalGenerator::STANDARD_TUNING[ string ], 0.5f );
    }
}
