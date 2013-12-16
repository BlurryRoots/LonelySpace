#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <BlurryRoots/Yanecos/Database.hpp>
#include <BlurryRoots/Yanecos/IData.hpp>

#include <memory>
#include <sstream>
#include <iostream>
#include <typeinfo>

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

        ~TestData2()
        {
            reporter << "Destroyed TestData2";
        }

        float f;
        double d;
        int i;
};


TEST_CASE( "Database", "[database]" )
{
    SECTION( "Add TestData1" )
    {
        reporter.str("");
        reporter.clear();        
        {
            BlurryRoots::Yanecos::Database database;
            REQUIRE_THROWS( database.GetById( 42 ) );

            auto entry = std::make_shared<TestData1>( 1337 );
            database.Add( entry );

            auto r = database.GetAllOfType<TestData1>();

            REQUIRE( r.size() == 1 );
            REQUIRE( r.front() == entry );
        }

        REQUIRE( reporter.str() == "Destroyed TestData1" );
    }

    SECTION( "Add TestData2" )
    {
        reporter.str("");
        reporter.clear();
        {
            BlurryRoots::Yanecos::Database database;
            REQUIRE_THROWS( database.GetById( 42 ) );

            auto entry = std::make_shared<TestData2>( 
                1.612f, 3.1415, 1337
            );
            database.Add( entry );

            {
                auto r = database.GetAllOfType<TestData1>();
                REQUIRE( r.size() == 0 );
            }

            {
                auto r = database.GetAllOfType<TestData2>();
                REQUIRE( r.size() == 1 );
                REQUIRE( r.front() == entry );
            }
        }

        REQUIRE( reporter.str() == "Destroyed TestData2" );
    }

    SECTION( "Add TestData1 and TestData2 and remove TestData1" )
    {
        reporter.str("");
        reporter.clear();
        {
            BlurryRoots::Yanecos::Database database;

            database.Add( std::make_shared<TestData1>( 
                23
            ));            
            REQUIRE( database.HasType<TestData1>() );

            database.Add( std::make_shared<TestData2>( 
                1.612f, 3.1415, 1337 
            ));
            REQUIRE( database.HasType<TestData2>() );

            {
                auto r = database.GetAllOfType<TestData1>();
                REQUIRE( r.size() == 1 );
                REQUIRE( r.front()->i == 23 );
            }

            {
                auto r = database.GetAllOfType<TestData2>();
                REQUIRE( r.size() == 1 );
                REQUIRE( r.front()->f == 1.612f );
            }

            {
                auto r = database.GetAllOfType<TestData1>();
                database.Remove( r.front() );
                REQUIRE_FALSE( database.HasType<TestData1>() );
            }            
            REQUIRE( reporter.str() == "Destroyed TestData1" );

            reporter.str("");
            reporter.clear();
        }

        REQUIRE( reporter.str() == "Destroyed TestData2" );
    }

    SECTION( "Add a bunch of stuff" )
    {
        const int AMOUNT = 13370;

        reporter.str("");
        reporter.clear();
        {
            BlurryRoots::Yanecos::Database database;

            for( int i = 0; i < AMOUNT; ++i )
            {
                if( (i + 1) % 2 == 0 )
                {
                    database.Add( std::make_shared<TestData1>( 
                        i + 1
                    ));
                }
                else
                {

                    database.Add( std::make_shared<TestData2>( 
                        (i + 1) * 1.612f,
                        (i + 1) * 3.1415,
                        (i + 1)
                    ));
                }
            }

            //
            {
                auto r = database.GetAllOfType<TestData1>();
                REQUIRE( r.size() == AMOUNT / 2 );
                REQUIRE( 
                    r.back()->i == 
                    (AMOUNT % 2 == 0
                        ? AMOUNT
                        : AMOUNT - 1) );
            }
            {
                auto r = database.GetAllWithTypeHash(
                    typeid( TestData1 ).hash_code()
                );
                REQUIRE( r.size() == AMOUNT / 2 );
            }

            //
            {
                auto r = database.GetAllOfType<TestData2>();
                REQUIRE( 
                    r.size() == 
                    (AMOUNT % 2 == 0
                        ? AMOUNT / 2
                        : AMOUNT / 2 + 1) );
                REQUIRE( 
                    r.back()->i == 
                    (AMOUNT % 2 != 0
                        ? AMOUNT
                        : AMOUNT - 1) );
            }
            {
                auto r = database.GetAllWithTypeHash(
                    typeid( TestData2 ).hash_code()
                );
                REQUIRE( 
                    r.size() == 
                    (AMOUNT % 2 == 0
                        ? AMOUNT / 2
                        : AMOUNT / 2 + 1) );
            }
        }
    }
}