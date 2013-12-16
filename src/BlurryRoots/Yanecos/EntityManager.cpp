
#include "EntityManager.hpp"

#include <set>
#include <algorithm>
#include <iostream>

namespace BlurryRoots
{
namespace Yanecos
{

std::size_t 
EntityManager::NextId()
{
    if( _ids[false].empty() )
    {
        _ids[false].push_back( ++_idCounter );
    }

    std::size_t id;
    id = _ids[false].front();
    _ids[false].pop_front();
    _ids[true].push_back( id );

    return id;
}

void
EntityManager::FreeId( std::size_t someId )
{
    auto it = _ids[true].begin();
    for( ; it != _ids[true].end(); ++it )
    {
        if( *it == someId )
        {
            break;
        }
    }

    if( it != _ids[true].end() )
    {
        _ids[true].erase( it );
        _ids[false].push_back( someId );
    }
}
       
std::size_t
EntityManager::CreateEntity( void )
{
    return this->CreateEntity( "*NO_TAG*" );
}

std::size_t
EntityManager::CreateEntity( std::string someTag )
{
    std::size_t id = this->NextId();

    _entityIdToDataIdLookup[id] =
        std::vector<std::size_t>();

    _entityIdToTagLookup[id] = someTag;

    if( _tagToEntityIdLookup.count( someTag ) == 0 )
    {
        _tagToEntityIdLookup[someTag] =
            std::vector<std::size_t>();
    }

    _tagToEntityIdLookup[someTag].push_back( id );

    return id;
}

void
EntityManager::SetEntityTag( std::size_t someId, std::string someTag )
{
    if( _entityIdToDataIdLookup.count( someId ) == 0 )
    {
        throw std::logic_error(
            "Entity with given id does not exist!"
        );
    }

    std::string oldTag = _entityIdToTagLookup[someId];
    _entityIdToTagLookup[someId] = someTag;

    auto it = std::find( 
        _tagToEntityIdLookup[oldTag].begin(),
        _tagToEntityIdLookup[oldTag].end(),
        someId 
    );
    if( it == _tagToEntityIdLookup[oldTag].end() )
    {
        throw std::logic_error(
            "Tag Error!"
        );        
    }
    _tagToEntityIdLookup[oldTag].erase( it );

    if( _tagToEntityIdLookup.count( someTag ) == 0 )
    {
        _tagToEntityIdLookup[someTag] =
            std::vector<std::size_t>();
    }
    _tagToEntityIdLookup[someTag].push_back( someId );
}

std::string
EntityManager::GetEntityTag( std::size_t someId )
{
    if( _entityIdToDataIdLookup.count( someId ) == 0 )
    {
        throw std::logic_error(
            "Entity with given id does not exist!"
        );
    }

    return _entityIdToTagLookup[someId];
}

std::vector<std::size_t>
EntityManager::GetEntitiesWithTag( std::string someTag )
{
    if( _tagToEntityIdLookup.count( someTag ) == 0 )
    {
        throw std::logic_error(
            "No such tag!"
        );
    }

    return _tagToEntityIdLookup[someTag];
}

std::vector<std::size_t>
EntityManager::GetEntitiesWithData( std::vector<std::size_t> someDataHashes )
{
    if( someDataHashes.empty() )
    {
        return std::vector<std::size_t>();
    }

    std::vector<std::size_t> result;
    for( auto entry : _entityIdToDataIdLookup )
    {
        bool hasAllTypes = false;
        std::size_t entityId = entry.first;
        std::vector<std::size_t>& entityDataIds = entry.second;

        std::size_t hashCount = 0;
        for( std::size_t dataId : entityDataIds )
        {
            std::size_t hash = _dataIdToDataTypeHashLookup[dataId];
            auto it = std::find(
                someDataHashes.begin(),
                someDataHashes.end(),
                hash
            );
            if( it != someDataHashes.end() // if this data is of required type
             && ++hashCount == someDataHashes.size() ) // and this entity has all required data
            {
                // end loop
                hasAllTypes = true;
                break;
            }
        }

        if( hasAllTypes )
        {
            result.push_back( entityId );
        }
    }

    return result;
}

void
EntityManager::DestroyEntity( std::size_t someId )
{
    if( _entityIdToDataIdLookup.count( someId ) == 0 )
    {
        throw std::logic_error(
            "Entity with given id does not exist!"
        );
    }

    for( auto it = _entityIdToDataIdLookup[someId].begin();
         it != _entityIdToDataIdLookup[someId].end();
         ++it )
    {
        _database->Remove( *it );
        _dataIdToDataTypeHashLookup.erase(
            _dataIdToDataTypeHashLookup.find( *it )
        );
    }

    _entityIdToDataIdLookup.erase(
        _entityIdToDataIdLookup.find( someId )
    );

    this->FreeId( someId );
}

}
}
