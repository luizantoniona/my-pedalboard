#include <gtest/gtest.h>

#include <cmath>
#include <memory>
#include <vector>

#include <QSignalSpy>

#include <Core/DSP/Effect/Utility/Gain.h>
#include <Core/DSP/EffectNode.h>
#include <Engine/Audio/AudioWorker.h>
#include <Platform/Null/NullAudioBackend.h>

namespace {

class WorkerFixture {

public:
    WorkerFixture() {
        auto owned = std::make_unique<Platform::NullAudioBackend>();
        backend = owned.get();
        worker = std::make_unique<Engine::AudioWorker>( std::move( owned ) );
        worker->refreshDevices();
        worker->setFrameBuffer( FRAMES );
        worker->start();
    }

    std::vector<float> render( const std::vector<float>& input ) {
        std::vector<float> output( input.size() * 2, 0.0f );
        backend->renderBlock( input.data(), output.data(), static_cast<uint32_t>( input.size() ) );
        return output;
    }

    static constexpr uint32_t FRAMES = 128;

    Platform::NullAudioBackend* backend{ nullptr };
    std::unique_ptr<Engine::AudioWorker> worker;
};

std::vector<float> ramp( uint32_t count ) {
    std::vector<float> samples( count );
    for ( uint32_t i = 0; i < count; i++ ) {
        samples[ i ] = static_cast<float>( i ) / static_cast<float>( count ) - 0.5f;
    }
    return samples;
}

} // namespace

TEST( AudioWorkerTest, PassesMonoInputToBothOutputChannels ) {
    WorkerFixture fixture;

    const std::vector<float> input = ramp( WorkerFixture::FRAMES );
    const std::vector<float> output = fixture.render( input );

    ASSERT_EQ( output.size(), input.size() * 2 );
    for ( uint32_t i = 0; i < WorkerFixture::FRAMES; i++ ) {
        EXPECT_FLOAT_EQ( output[ 2 * i ], input[ i ] );
        EXPECT_FLOAT_EQ( output[ 2 * i + 1 ], input[ i ] );
    }
}

TEST( AudioWorkerTest, SilentInputProducesSilentOutput ) {
    WorkerFixture fixture;

    const std::vector<float> input( WorkerFixture::FRAMES, 0.0f );
    const std::vector<float> output = fixture.render( input );

    for ( float sample : output ) {
        EXPECT_FLOAT_EQ( sample, 0.0f );
    }
}

TEST( AudioWorkerTest, NullInputDoesNotCrash ) {
    WorkerFixture fixture;

    std::vector<float> output( WorkerFixture::FRAMES * 2, 1.0f );
    fixture.backend->renderBlock( nullptr, output.data(), WorkerFixture::FRAMES );

    EXPECT_FLOAT_EQ( output.front(), 0.0f );
}

TEST( AudioWorkerTest, EffectInTheGraphChangesTheOutput ) {
    WorkerFixture fixture;

    auto gain = std::make_unique<Core::Gain>();
    gain->setParameter( "gainDb", -6.0f );
    fixture.worker->graph().addNode( "gain", std::make_shared<Core::EffectNode>( std::move( gain ) ) );

    const std::vector<float> input( WorkerFixture::FRAMES, 1.0f );
    std::vector<float> output;
    for ( int block = 0; block < 80; block++ ) {
        output = fixture.render( input );
    }

    EXPECT_NEAR( output[ 0 ], 0.5011f, 1.0e-3f );
    EXPECT_NEAR( output[ 1 ], 0.5011f, 1.0e-3f );
}

TEST( AudioWorkerTest, GrantedBufferLargerThanRequestedDoesNotOverflow ) {
    WorkerFixture fixture;

    // The driver can hand back more frames than asked for; the callback must
    // never write past the buffers sized at open time.
    const std::vector<float> oversized( WorkerFixture::FRAMES * 4, 0.5f );
    std::vector<float> output( oversized.size() * 2, -1.0f );

    fixture.backend->renderBlock( oversized.data(), output.data(), static_cast<uint32_t>( oversized.size() ) );

    EXPECT_FLOAT_EQ( output[ 0 ], 0.5f );
    EXPECT_FLOAT_EQ( output.back(), 0.0f );
}
