
#include "Database.hpp"

#include <stdexcept>
#include <sstream>
#include <iostream>

namespace BlurryRoots
{

namespace Yanecos
{

std::size_t 
Database::NextId()
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
Database::FreeId( std::size_t someId )
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

Database::Database()
    : _idCounter( 0 )
{   
    _ids[true] = std::deque<std::size_t>();
    _ids[false] = std::deque<std::size_t>();
}

std::size_t 
Database::Add( std::shared_ptr<IData> someData )
{
    if( someData == nullptr )
    {
        throw std::logic_error( "someData == nullptr" );
    }

    std::type_index type = typeid( *someData );
    std::size_t newId = this->NextId();

    if( _typeToIdLookup.count( type ) == 0 )
    {
        _typeToIdLookup[type] = std::vector<std::size_t>();
    }

    _typeToIdLookup[type].push_back( newId );
    _idToTypeLookup[newId] = type.hash_code();
    _data[newId] = someData;

    return newId;
}

std::shared_ptr<IData> 
Database::GetById( std::size_t someId )
{
    if( _data.count( someId ) == 0 )
    {
        std::stringstream ss;
        ss << "No data with id " << someId << "registered!";

        throw std::logic_error( ss.str() );
    }

    return _data[someId];
}

std::vector<std::size_t>
Database::GetAllWithTypeHash( type_index_hash_code_t someHash )
{
    std::vector<std::size_t> result;
    for( auto entry : _typeToIdLookup )
    {
        if( entry.first.hash_code() == someHash )
        {
            result = entry.second;
        }
    }

    return result;
}

type_index_hash_code_t
Database::GetTypeHash( std::size_t someId )
{
    for( auto entry : _idToTypeLookup )
    {
        if( entry.first == someId )
        {
            return entry.second;
        }
    }

    throw std::logic_error( "No data with given id found!" );
}

void
Database::Remove( std::size_t someId )
{
    if( _data.count( someId ) == 0 )
    {
        return;
    }

    auto dataIt = _data.find( someId );
    if( dataIt == _data.end() )
    {
        throw std::logic_error( 
            "In _data: ID has not been found!" 
        );
    }
    _data.erase( dataIt );

    auto typeIt = _idToTypeLookup.find( someId );
    if( typeIt == _idToTypeLookup.end() )
    {
        throw std::logic_error( 
            "In _idToTypeLookup: ID has not been found!" 
        );
    }
    type_index_hash_code_t hashCode = typeIt->second;
    _idToTypeLookup.erase( typeIt );

    auto idVectorIt = _typeToIdLookup.begin();
    while( idVectorIt != _typeToIdLookup.end() )
    {
        if( idVectorIt->first.hash_code() == hashCode )
        {
            break;
        }

        ++idVectorIt;
    }
    auto it = idVectorIt->second.begin(); 
    while( it != idVectorIt->second.end() )
    {
        if( *it == someId )
        {
            break;
        }

        ++it;
    }
    if( it == idVectorIt->second.end() )
    {
        throw std::logic_error( 
            "In _typeToIdLookup: ID has not been found!"
        );
    }
    idVectorIt->second.erase( it );
    if( idVectorIt->second.empty() )
    {
        _typeToIdLookup.erase( idVectorIt );
    }

    this->FreeId( someId );
}

void
Database::Remove( std::shared_ptr<IData> someData )
{
    if( someData == nullptr )
    {
        throw std::logic_error(
            "someData == nullptr"
        );
    }
    
    bool found = false;
    std::size_t id = 0;
    for( auto entry : _data )
    {
        if( entry.second == someData )
        {
            id = entry.first;
            found = true;
            break;
        }
    }

    if( ! found )
    {
        throw std::logic_error(
            "Trying to remove data, not contained in database!"
        );
    }

    this->Remove( id );
}

}

}
