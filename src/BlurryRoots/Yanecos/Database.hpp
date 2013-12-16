
#include "IData.hpp"

#include <map>
#include <typeinfo>
#include <string>
#include <vector>
#include <typeindex>
#include <memory>
#include <deque>

#ifndef _DATABASE_
#define _DATABASE_

namespace BlurryRoots
{

namespace Yanecos
{

typedef
    std::size_t
    type_index_hash_code_t;

class Database
{
    private:
        
        std::map<std::size_t, type_index_hash_code_t> _idToTypeLookup;

        std::map<std::type_index, std::vector<std::size_t>> _typeToIdLookup;

        std::map<std::size_t, std::shared_ptr<IData>> _data;

        std::size_t _idCounter;
        std::map<bool, std::deque<std::size_t>> _ids;

        std::size_t 
        NextId();

        void
        FreeId( std::size_t );

    public:

        Database();

        template<class T>
        bool 
        HasType( void )
        {
            std::type_index type = typeid( T );
            for( auto entry : _typeToIdLookup )
            {
                if( entry.first == type )
                {
                    return true;
                }
            }

            return false;
        }

        template<class T>
        std::vector<std::shared_ptr<T>> 
        GetAllOfType( void )
        {
            static_assert(
                std::is_convertible<T*, IData*>::value,
                "Given Type does not implement IData!"
            );

            std::vector<std::shared_ptr<T>> result;

            if( ! this->HasType<T>() )
            {
                return result;
            }

            std::type_index type = typeid( T );

            for( auto id : _typeToIdLookup[type] )
            {
                result.push_back( 
                    std::static_pointer_cast<T>( _data[id] )
                );
            }

            return result;
        }

        std::vector<std::size_t>
        GetAllWithTypeHash( type_index_hash_code_t someHash );

        template<class T>
        std::vector<std::size_t> 
        GetIdsWithType( void )
        {
            static_assert(
                std::is_convertible<T*, IData*>::value,
                "Given Type does not implement IData!"
            );

            std::vector<std::size_t> result;

            if( ! this->HasType<T>() )
            {
                return result;
            }

            std::type_index type = typeid( T );

            for( auto id : _typeToIdLookup[type] )
            {
                result.push_back( id );
            }

            return result;
        }

        std::size_t 
        Add( std::shared_ptr<IData> someData );

        std::shared_ptr<IData> 
        GetById( std::size_t someId );

        type_index_hash_code_t
        GetTypeHash( std::size_t someId );

        void
        Remove( std::size_t someId );

        void
        Remove( std::shared_ptr<IData> someData );
};

}

}

#endif
