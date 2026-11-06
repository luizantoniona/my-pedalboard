#include <set>
#include <string>

#include <gtest/gtest.h>

#include <Core/Project/NodeRegistry.h>
#include <Core/Project/PedalRegistry.h>

namespace {

const Core::ParameterInfo* findParameter( const Core::DSPModule& module, const std::string& name ) {
    for ( const Core::ParameterInfo& info : module.parameters() ) {
        if ( info.name == name ) {
            return &info;
        }
    }
    return nullptr;
}

} // namespace

TEST( PedalRegistryTest, EveryBuiltinPedalBuildsItsAlgorithm ) {
    const Core::PedalRegistry& registry = Core::PedalRegistry::withBuiltinPedals();

    ASSERT_FALSE( registry.ids().empty() );
    for ( const std::string& id : registry.ids() ) {
        const Core::PedalDefinition* definition = registry.definition( id );
        ASSERT_NE( definition, nullptr ) << id;

        const std::unique_ptr<Core::DSPModule> module = registry.create( id );
        ASSERT_NE( module, nullptr ) << id;
        EXPECT_EQ( module->typeName(), definition->algorithm ) << id;
        EXPECT_FALSE( definition->displayName.empty() ) << id;
    }
}

TEST( PedalRegistryTest, IdsAreUnique ) {
    const std::vector<std::string> ids = Core::PedalRegistry::withBuiltinPedals().ids();

    EXPECT_EQ( std::set<std::string>( ids.begin(), ids.end() ).size(), ids.size() );
}

TEST( PedalRegistryTest, EveryOverrideTargetsARealParameterInsideItsRange ) {
    const Core::PedalRegistry& registry = Core::PedalRegistry::withBuiltinPedals();

    for ( const std::string& id : registry.ids() ) {
        const std::unique_ptr<Core::DSPModule> module = registry.create( id );
        ASSERT_NE( module, nullptr ) << id;

        for ( const Core::ParameterOverride& entry : registry.definition( id )->overrides ) {
            const Core::ParameterInfo* info = findParameter( *module, entry.name );
            ASSERT_NE( info, nullptr ) << id << "." << entry.name;
            EXPECT_GE( entry.value, info->minimum ) << id << "." << entry.name;
            EXPECT_LE( entry.value, info->maximum ) << id << "." << entry.name;
        }
    }
}

TEST( PedalRegistryTest, ClonesOfOneAlgorithmStartWithDifferentSettings ) {
    const Core::PedalRegistry& registry = Core::PedalRegistry::withBuiltinPedals();

    const std::unique_ptr<Core::DSPModule> room = registry.create( "RoomReverb" );
    const std::unique_ptr<Core::DSPModule> hall = registry.create( "HallReverb" );
    ASSERT_NE( room, nullptr );
    ASSERT_NE( hall, nullptr );

    EXPECT_EQ( room->typeName(), hall->typeName() );
    EXPECT_NEAR( room->parameter( "size" ), 0.3f, 1.0e-6f );
    EXPECT_NEAR( hall->parameter( "size" ), 0.85f, 1.0e-6f );
}

TEST( PedalRegistryTest, ParametersWithoutAnOverrideKeepTheirDefaults ) {
    const std::unique_ptr<Core::DSPModule> overdrive = Core::PedalRegistry::withBuiltinPedals().create( "Overdrive" );
    ASSERT_NE( overdrive, nullptr );

    EXPECT_NEAR( overdrive->parameter( "drive" ), 4.0f, 1.0e-6f );
    EXPECT_NEAR( overdrive->parameter( "level" ), -6.0f, 1.0e-3f );
}

TEST( PedalRegistryTest, UnknownPedalCreatesNothing ) {
    EXPECT_EQ( Core::PedalRegistry::withBuiltinPedals().create( "NoSuchPedal" ), nullptr );
    EXPECT_EQ( Core::PedalRegistry::withBuiltinPedals().definition( "NoSuchPedal" ), nullptr );
}

TEST( PedalRegistryTest, CustomPedalRegistersAtRuntime ) {
    Core::PedalRegistry registry( Core::NodeRegistry::withBuiltinTypes() );
    registry.registerPedal( { "LongEcho", "Long Echo", "Delay", { { "time", 1200.0f }, { "feedback", 0.6f } } } );

    ASSERT_EQ( registry.ids().size(), 1u );
    const std::unique_ptr<Core::DSPModule> module = registry.create( "LongEcho" );
    ASSERT_NE( module, nullptr );
    EXPECT_NEAR( module->parameter( "time" ), 1200.0f, 1.0e-3f );
    EXPECT_NEAR( module->parameter( "feedback" ), 0.6f, 1.0e-6f );
}

TEST( PedalRegistryTest, PedalWithAnUnknownAlgorithmCreatesNothing ) {
    Core::PedalRegistry registry( Core::NodeRegistry::withBuiltinTypes() );
    registry.registerPedal( { "Ghost", "Ghost", "NoSuchAlgorithm", {} } );

    EXPECT_EQ( registry.create( "Ghost" ), nullptr );
}

TEST( PedalRegistryTest, RegisteringAnExistingIdReplacesItWithoutDuplicatingTheListing ) {
    Core::PedalRegistry registry( Core::NodeRegistry::withBuiltinTypes() );
    registry.registerPedal( { "Echo", "Echo", "Delay", { { "time", 200.0f } } } );
    registry.registerPedal( { "Echo", "Echo", "Delay", { { "time", 700.0f } } } );

    EXPECT_EQ( registry.ids().size(), 1u );
    EXPECT_NEAR( registry.create( "Echo" )->parameter( "time" ), 700.0f, 1.0e-3f );
}
