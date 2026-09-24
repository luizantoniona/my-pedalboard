#include <gtest/gtest.h>

#include <QCoreApplication>

// The engine suite needs a running event loop: the analysis timer that feeds
// the tuner and the meters only fires while events are being processed.
int main( int argc, char* argv[] ) {
    QCoreApplication application( argc, argv );
    ::testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
