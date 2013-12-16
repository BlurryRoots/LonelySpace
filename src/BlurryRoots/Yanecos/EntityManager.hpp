
#include "Database.hpp"
#include "IEntity.hpp"

#include <map>
#include <vector>
#include <memory>
#include <stdexcept>
#include <deque>

#ifndef _ENTITYMANAGER_
#define _ENTITYMANAGER_
    
namespace BlurryRoots
{

namespace Yanecos
{

class EntityManager
{
    private:

        std::shared_ptr<Database> _database;

        std::map<std::size_t, std::vector<std::size_t>> _entityIdToDataIdLookup;

        std::map<std::size_t, std::size_t> _dataIdToDataTypeHashLookup;

        std::map<std::string, std::vector<std::size_t>> _tagToEntityIdLookup;
        
        std::map<std::size_t, std::string> _entityIdToTagLookup;

        std::size_t _idCounter;
        std::map<bool, std::deque<std::size_t>> _ids;

        std::size_t 
        NextId();

        void
        FreeId( std::size_t );

        class EntityImpl
            : public IEntity
        {
            private:

                EntityManager& _manager;

                std::size_t _id;

            protected:

                std::shared_ptr<IData> GetDataWithTypeHash( std::size_t )
                {
                    throw std::logic_error( "Not yet implemented!" );
                }

            public:
        
                EntityImpl( EntityManager& someManager, std::size_t someId )
                    : _manager( someManager ),
                      _id( someId )
                {                    
                }

                void 
                AddData( std::shared_ptr<IData> someData )
                {                    
                    throw std::logic_error( "Not yet implemented!" );
                }

            
                std::size_t
                GetId()
                {
                    throw std::logic_error( "Not yet implemented!" );                    
                }
        };

        friend EntityImpl;

    public:

        EntityManager()
            : _database( std::make_shared<Database>() ), 
                // just create a new one for now, maybe later
                // entity manager can be fed with a preset database
                // and work with already created data (then i might want
                // to serialize the entity manager too, to keep entity data connetion)
              _idCounter( 0 )
        {
            _ids[true] = std::deque<std::size_t>();
            _ids[false] = std::deque<std::size_t>();
        }
        
        std::size_t
        CreateEntity( void );

        std::size_t
        CreateEntity( std::string someTag );

        void
        DestroyEntity( std::size_t someId );

        void
        SetEntityTag( std::size_t someId, std::string someTag );

        std::string
        GetEntityTag( std::size_t someId );

        std::vector<std::size_t>
        GetEntitiesWithTag( std::string someTag );

        template<class T>
        bool HasEntityDataOfType( std::size_t someId )
        {
            static_assert(
                std::is_convertible<T*, IData*>::value,
                "Given type does not implement IData!"
            );

            if( _entityIdToDataIdLookup.count( someId ) == 0 )
            {
                throw std::logic_error(
                    "Entity with given id does not exist!"
                );
            }

            // create hash for type
            std::size_t hash = typeid( T ).hash_code();
            // fetch data ids for entity
            auto entityDataIdVector = _entityIdToDataIdLookup[someId];
            // check every hash code of every data attached to 
            // given entity, trying to check if entity already
            // holds data of the type which produces the above hash code
            for( auto dataId : entityDataIdVector )
            {
                if( _dataIdToDataTypeHashLookup[dataId] == hash )
                {
                    return true;
                }
            }

            return false;
        }

        template<class T>
        std::shared_ptr<T> AddDataToEntity( std::size_t someId, T someData )
        {
            static_assert(
                std::is_convertible<T*, IData*>::value,
                "Given type does not implement IData!"
            );

            if( _entityIdToDataIdLookup.count( someId ) == 0 )
            {
                throw std::logic_error(
                    "Entity with given id does not exist!"
                );
            }

            if( this->HasEntityDataOfType<T>( someId ) )
            {
                throw std::logic_error(
                    "Entity already has data of given type!"
                );                
            }

            std::shared_ptr<T> dataPtr =
                std::make_shared<T>( someData );
            std::size_t hash = typeid( T ).hash_code();
            std::size_t dataId = _database->Add( dataPtr );

            _entityIdToDataIdLookup[someId].push_back( dataId );
            _dataIdToDataTypeHashLookup[dataId] = hash;

            return dataPtr;
        }

        template<class T>
        std::shared_ptr<T> AddDataToEntity( std::size_t someId )
        {
            static_assert(
                std::is_convertible<T*, IData*>::value,
                "Given type does not implement IData!"
            );

            static_assert(
                std::is_default_constructible<T>::value,
                "Given type has to have a default constructor!"
            );

            if( _entityIdToDataIdLookup.count( someId ) == 0 )
            {
                throw std::logic_error(
                    "Entity with given id does not exist!"
                );
            }

            if( this->HasEntityDataOfType<T>( someId ) )
            {
                throw std::logic_error(
                    "Entity already has data of given type!"
                );                
            }

            std::shared_ptr<T> dataPtr =
                std::make_shared<T>();
            std::size_t hash = typeid( T ).hash_code();
            std::size_t dataId = _database->Add( dataPtr );

            _entityIdToDataIdLookup[someId].push_back( dataId );
            _dataIdToDataTypeHashLookup[dataId] = hash;

            return dataPtr;
        }

        template<class T>
        std::shared_ptr<T> GetEntityDataOfType( std::size_t someId )
        {
            static_assert(
                std::is_convertible<T*, IData*>::value,
                "Given type does not implement IData!"
            );

            if( _entityIdToDataIdLookup.count( someId ) == 0 )
            {
                throw std::logic_error(
                    "Entity with given id does not exist!"
                );
            }

            std::size_t hash = typeid( T ).hash_code();

            auto entityDataIdVector =
                _entityIdToDataIdLookup[someId];
            for( auto dataId : entityDataIdVector )
            {
                if( _dataIdToDataTypeHashLookup[dataId] == hash )
                {
                    return std::static_pointer_cast<T>(
                        _database->GetById( dataId )
                    );
                }
            }

            throw std::logic_error(
                "Entity does not have data of given type!"
            );
        }

        template<class T>
        std::vector<std::size_t>
        GetEntitiesWithData( void )
        {
            static_assert(
                std::is_convertible<T*, IData*>::value,
                "Given type does not implement IData!"
            );

            std::vector<std::size_t> result;
            /*for( auto entry : _entityIdToDataIdLookup )
            {
                std::size_t entityId = entry.first;
                if( this->HasEntityDataOfType<T>( entityId ) )
                {
                    result.push_back( entityId );
                }
            }*/
            std::size_t hash = typeid( T ).hash_code();
            for( auto entry : _entityIdToDataIdLookup )
            {
                std::size_t entityId = entry.first;
                auto entityDataIds = entry.second;
                for( auto dataId : entityDataIds )
                {
                    if( _dataIdToDataTypeHashLookup[dataId] == hash )
                    {
                        result.push_back( entityId );
                    }
                }
            }

            return result;
        }

        std::vector<std::size_t>
        GetEntitiesWithData( std::vector<std::size_t> someDataHashes );

        template<class T>
        void RemoveDataFromEntity( std::size_t someId )
        {
            static_assert(
                std::is_convertible<T*, IData*>::value,
                "Given type does not implement IData!"
            );

            if( _entityIdToDataIdLookup.count( someId ) == 0 )
            {
                throw std::logic_error(
                    "Entity with given id does not exist!"
                );
            }

            if( ! this->HasEntityDataOfType<T>( someId ) )
            {
                throw std::logic_error(
                    "Entity does not have data of given type!"
                );                
            }
            
            // create hash for type
            std::size_t hash = typeid( T ).hash_code();
            // fetch data ids for entity
            auto entityDataIdVector = _entityIdToDataIdLookup[someId];
            // check every hash code of every data attached to 
            // given entity, trying to check if entity already
            // holds data of the type which produces the above hash code
            std::size_t requestedDataId;
            for( auto dataId : entityDataIdVector )
            {
                if( _dataIdToDataTypeHashLookup[dataId] == hash )
                {
                    requestedDataId = dataId;
                }
            }

            _database->Remove( requestedDataId );
            {
                auto it =
                    _dataIdToDataTypeHashLookup.find( requestedDataId );
                _dataIdToDataTypeHashLookup.erase( it );
            }
            {
                auto it = _entityIdToDataIdLookup[someId].begin();
                while( it != _entityIdToDataIdLookup[someId].end() )
                {
                    if( *it == requestedDataId )
                    {
                        break;
                    }
                }

                if( it == _entityIdToDataIdLookup[someId].end() )
                {
                    throw std::logic_error(
                        "Internal error: Entity has not been created correctly!"
                    );
                }

                _entityIdToDataIdLookup[someId].erase( it );
            }
        }
};

}

}

#endif
