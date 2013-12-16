#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <BlurryRoots/Yanecos/IData.hpp>
#include <BlurryRoots/Yanecos/Database.hpp>
#include <BlurryRoots/Yanecos/EntityManager.hpp>

#include <memory>
#include <sstream>
#include <iostream>

std::stringstream reporter;

class NonDataClass
{};

class TestData1
    : public BlurryRoots::Yanecos::IData
{
    private:
    public:

        TestData1( int someV )
            : i( someV )
        {}

        TestData1()
            : TestData1( 0 )
        {}

        ~TestData1()
        {
            reporter << "Destroyed TestData1";
        }

        int i;
};

class TestData2
    : public BlurryRoots::Yanecos::IData
{
    private:
    public:

        TestData2( float someF, double someD, int someI )
            : f( someF ),
              d( someD ),
              i( someI )
        {}

        TestData2()
            : TestData2( 0.0f, 0.0, 0 )
        {}

        ~TestData2()
        {
            reporter << "Destroyed TestData2";
        }

        float f;
        double d;
        int i;
};

class TestData3
    : public BlurryRoots::Yanecos::IData
{
    private:
    public:

        TestData3( std::string someS )
            : s( someS )
        {}

        ~TestData3()
        {
            reporter << "Destroyed TestData3";
        }

        std::string s;
};

TEST_CASE( "EntityManager", "[entity]" )
{
    SECTION( "Create entity" )
    {
        BlurryRoots::Yanecos::EntityManager manager;

        REQUIRE( manager.CreateEntity() == 1 );
    }

    SECTION( "Create and remove entity" )
    {
        BlurryRoots::Yanecos::EntityManager manager;

        std::size_t id = manager.CreateEntity();
        REQUIRE( id == 1 );

        manager.DestroyEntity( id );
        REQUIRE_THROWS( manager.DestroyEntity( id ) );
    }

    SECTION( "Add data to entity" )
    {
        reporter.str( "" );
        reporter.clear();
        {
            BlurryRoots::Yanecos::EntityManager manager;

            std::size_t id = 1337;

            // Cannot check non exisiting entity
            REQUIRE_THROWS( manager.HasEntityDataOfType<TestData1>( id ) );
            // Cannot add stuff to nonexisting entity
            REQUIRE_THROWS( manager.AddDataToEntity( id, TestData1( 42 ) ) );
            REQUIRE( reporter.str() == "Destroyed TestData1" );
            reporter.str( "" );
            reporter.clear();

            id = manager.CreateEntity();
            REQUIRE( id == 1 );

            // Here compilation will fail due to static assertion magic
            // manager.AddDataToEntity( id, NonDataClass() );

            REQUIRE_FALSE( manager.HasEntityDataOfType<TestData1>( id ) );

            manager.AddDataToEntity( id, TestData1( 42 ) );
            REQUIRE( reporter.str() == "Destroyed TestData1" );
            reporter.str( "" );
            reporter.clear();

            REQUIRE( manager.HasEntityDataOfType<TestData1>( id ) );

            REQUIRE_THROWS( manager.AddDataToEntity( id, TestData1( 42 ) ) );
            REQUIRE( reporter.str() == "Destroyed TestData1" );
            reporter.str( "" );
            reporter.clear();

            {
                auto data = manager.GetEntityDataOfType<TestData1>( id );
                REQUIRE( data->i == 42 );
            }

            REQUIRE_FALSE( manager.HasEntityDataOfType<TestData2>( id ) );

            manager.AddDataToEntity( id, TestData2( 1.612f, 3.1415, 1337 ) );
            REQUIRE( reporter.str() == "Destroyed TestData2" );
            reporter.str( "" );
            reporter.clear();

            REQUIRE( manager.HasEntityDataOfType<TestData2>( id ) );

            REQUIRE_THROWS( manager.AddDataToEntity( id, TestData2( 1.612f, 3.1415, 1337 ) ) );
            REQUIRE( reporter.str() == "Destroyed TestData2" );
            reporter.str( "" );
            reporter.clear();

            {
                auto data = manager.GetEntityDataOfType<TestData2>( id );
                REQUIRE( data->d == 3.1415 );
            }

            manager.RemoveDataFromEntity<TestData1>( id );
            REQUIRE( reporter.str() == "Destroyed TestData1" );
            reporter.str( "" );
            reporter.clear();
            REQUIRE_FALSE( manager.HasEntityDataOfType<TestData1>( id ) );

            REQUIRE( manager.HasEntityDataOfType<TestData2>( id ) );
        }
    }

    SECTION( "Entity tag" )
    {
        const int AMOUNT = 200;
        const std::string TAG = "Keks";

        BlurryRoots::Yanecos::EntityManager manager;

        // AMOUNT / 2 + 1
        {
            std::size_t id = manager.CreateEntity();
            manager.SetEntityTag( id, TAG );
            REQUIRE( manager.GetEntityTag( id ) == TAG );
        }

        // AMOUNT / 2 + 2
        {
            std::size_t id = manager.CreateEntity( TAG );
            REQUIRE( manager.GetEntityTag( id ) == TAG );
        }

        for( int i = 0; i < AMOUNT; ++i )
        {
            std::size_t id = manager.CreateEntity();

            if( (i + 1) % 2 == 0 )
            {
                manager.SetEntityTag( id, TAG );
                REQUIRE( manager.GetEntityTag( id ) == TAG );
            }
        }
        
        auto ids = manager.GetEntitiesWithTag( TAG );
        REQUIRE( ids.size() == (AMOUNT / 2 + 2) );
    }

    SECTION( "Add data with default constructor" )
    {
        BlurryRoots::Yanecos::EntityManager manager;

        std::size_t id = manager.CreateEntity();

        {
            auto data = manager.AddDataToEntity<TestData1>( id );
            data->i = 42;
        }
        REQUIRE( manager.HasEntityDataOfType<TestData1>( id ) );

        {
            auto data = manager.AddDataToEntity<TestData2>( id );
            data->i = 3;
            data->f = 2.123f;
            data->d = 8234.42803;
        }
        REQUIRE( manager.HasEntityDataOfType<TestData2>( id ) );
    }

    SECTION( "Get entities grouped by data." )
    {
        const int AMOUNT = 10000;

        BlurryRoots::Yanecos::EntityManager manager;

        std::size_t lastId = 0;
        for( int i = 0; i < AMOUNT; ++i )
        {
            lastId = manager.CreateEntity();

            manager.AddDataToEntity( lastId, TestData1( i + 1 ) );

            if( (i + 1) % 2 == 0 )
            {
                manager.AddDataToEntity( lastId, TestData2( 
                    (i + 1) * 1.612f,
                    (i + 1) * 3.1415,
                    (i + 1)
                ));
            }
        }

        // add TestData3 to the last entity created
        {
            manager.AddDataToEntity( lastId, TestData3( "hanswurst" ) );
            auto r = manager.GetEntitiesWithData(
                std::vector<std::size_t>{
                    typeid( TestData1 ).hash_code(),
                    typeid( TestData2 ).hash_code(),
                    typeid( TestData3 ).hash_code()
                }
            );

            REQUIRE( r.size() == 1 );
            REQUIRE( r.front() == lastId );
        }

        // all with TestData1
        {
            auto r =
                manager.GetEntitiesWithData<TestData1>();
            REQUIRE( r.size() == AMOUNT );
        }

        // all with TestData2
        {
            auto r =
                manager.GetEntitiesWithData<TestData2>();
            REQUIRE( r.size() == 
                     (AMOUNT % 2 == 0
                        ? AMOUNT / 2
                        : AMOUNT / 2 + 1));
        }

        // all with TestData1 and TestData2
        {
            auto r = 
                manager.GetEntitiesWithData( std::vector<std::size_t>{
                    typeid( TestData1 ).hash_code(),
                    typeid( TestData2 ).hash_code()
                });
            REQUIRE( r.size() == 
                     (AMOUNT % 2 == 0
                        ? AMOUNT / 2
                        : AMOUNT / 2 + 1));
        }
    }
}