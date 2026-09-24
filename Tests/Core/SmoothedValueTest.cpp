#include <gtest/gtest.h>

#include <Core/Utils/SmoothedValue.h>

TEST( SmoothedValueTest, DoesNotJumpToTarget ) {
    Core::SmoothedValue value;
    value.prepare( 48000, 20.0f );
    value.snapTo( 0.0f );
    value.setTarget( 1.0f );

    EXPECT_LT( value.next(), 0.1f );
}

TEST( SmoothedValueTest, ConvergesToTarget ) {
    Core::SmoothedValue value;
    value.prepare( 48000, 5.0f );
    value.snapTo( 0.0f );
    value.setTarget( 1.0f );

    for ( int i = 0; i < 48000; i++ ) {
        value.next();
    }

    EXPECT_NEAR( value.current(), 1.0f, 1.0e-4f );
}

TEST( SmoothedValueTest, SnapIsImmediate ) {
    Core::SmoothedValue value;
    value.prepare( 48000 );
    value.snapTo( 0.5f );

    EXPECT_FLOAT_EQ( value.next(), 0.5f );
}
