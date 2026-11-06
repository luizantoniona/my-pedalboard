#include <gtest/gtest.h>

#include <Core/Utils/RingBuffer.h>

TEST( RingBufferTest, PopsWhatWasPushed ) {
    Core::RingBuffer<float, 8> buffer;

    ASSERT_TRUE( buffer.push( 1.0f ) );
    ASSERT_TRUE( buffer.push( 2.0f ) );

    EXPECT_EQ( buffer.available(), 2u );
    EXPECT_FLOAT_EQ( *buffer.pop(), 1.0f );
    EXPECT_FLOAT_EQ( *buffer.pop(), 2.0f );
    EXPECT_FALSE( buffer.pop().has_value() );
}

TEST( RingBufferTest, DropsWhenFullInsteadOfBlocking ) {
    Core::RingBuffer<float, 4> buffer;
    const float values[ 8 ] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    EXPECT_EQ( buffer.writeBlock( values, 8 ), buffer.capacity() );
}

TEST( RingBufferTest, ReadBlockStopsAtWhatIsAvailable ) {
    Core::RingBuffer<float, 16> buffer;
    const float values[ 3 ] = { 1, 2, 3 };
    buffer.writeBlock( values, 3 );

    float destination[ 5 ] = { 0, 0, 0, 0, 0 };
    EXPECT_EQ( buffer.readBlock( destination, 5 ), 3u );
    EXPECT_FLOAT_EQ( destination[ 2 ], 3.0f );
}
