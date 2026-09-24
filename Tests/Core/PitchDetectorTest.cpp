#include <gtest/gtest.h>

#include <cmath>
#include <vector>

#include <Core/Analysis/NoteHelper.h>
#include <Core/Analysis/PitchDetector.h>

namespace {

std::vector<float> sine( float frequency, uint32_t sampleRate, uint32_t count, float amplitude = 0.5f ) {
    std::vector<float> samples( count );
    for ( uint32_t i = 0; i < count; i++ ) {
        samples[ i ] = amplitude * std::sin( 2.0f * 3.14159265f * frequency * static_cast<float>( i ) / static_cast<float>( sampleRate ) );
    }
    return samples;
}

} // namespace

TEST( PitchDetectorTest, DetectsLowE ) {
    Core::PitchDetector detector;
    detector.prepare( 48000 );

    const std::vector<float> samples = sine( 82.41f, 48000, detector.windowSize() );
    const Core::PitchDetector::Result result = detector.analyze( samples.data(), detector.windowSize() );

    ASSERT_TRUE( result.voiced );
    EXPECT_NEAR( result.frequency, 82.41f, 1.5f );
}

TEST( PitchDetectorTest, DetectsA440 ) {
    Core::PitchDetector detector;
    detector.prepare( 48000 );

    const std::vector<float> samples = sine( 440.0f, 48000, detector.windowSize() );
    const Core::PitchDetector::Result result = detector.analyze( samples.data(), detector.windowSize() );

    ASSERT_TRUE( result.voiced );
    EXPECT_NEAR( result.frequency, 440.0f, 6.0f );
}

TEST( PitchDetectorTest, SilenceIsUnvoiced ) {
    Core::PitchDetector detector;
    detector.prepare( 48000 );

    const std::vector<float> samples( detector.windowSize(), 0.0f );
    const Core::PitchDetector::Result result = detector.analyze( samples.data(), detector.windowSize() );

    EXPECT_FALSE( result.voiced );
}

TEST( PitchDetectorTest, RejectsShortInput ) {
    Core::PitchDetector detector;
    detector.prepare( 48000 );

    const std::vector<float> samples( 64, 0.5f );
    const Core::PitchDetector::Result result = detector.analyze( samples.data(), 64 );

    EXPECT_FALSE( result.voiced );
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
