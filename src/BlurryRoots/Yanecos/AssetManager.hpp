
#include <typeinfo>
#include <typeindex>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

#ifndef _ASSETMANAGER_
#define _ASSETMANAGER_

namespace BlurryRoots
{

namespace Yanecos
{
    
class AssetManager
{
    private:

        std::map<std::type_index, std::map<std::string, std::shared_ptr<void>>> _data;

    public:

        template<class T>
        void
        Add( std::string someKey, std::shared_ptr<T> someData )
        {
            if( someData == nullptr )
            {
                throw std::logic_error( "Cannot store unvalid data!" );
            }

            std::type_index type = typeid( T );

            if( _data.count( type ) == 0 )
            {
                // create new storage for given type
                _data[type] = std::map<std::string, std::shared_ptr<void>>();
            }

            if( _data[type].count( someKey ) > 0 )
            {
                throw std::logic_error( "AssetManager already contains key!" );
            }

            _data[type][someKey] = someData;
        }

        template<class T>
        std::shared_ptr<T>
        GetAs( std::string someKey )
        {
            std::type_index type = typeid( T );
            if( _data.count( type ) == 0 )
            {
                throw std::logic_error( "No assets with given type!" );
            }

            if( _data[type].count( someKey ) == 0 )
            {
                throw std::logic_error( "No asset of given type for given key!" );
            }

            return std::static_pointer_cast<T>(
                _data[type][someKey]
            );
        }

        template<class T>
        void
        Remove( std::string someKey )
        {
            std::type_index type = typeid( T );
            if( _data.count( type ) == 0 )
            {
                throw std::logic_error( "No assets with given type!" );
            }

            auto it = _data[type].find( someKey );
            if( it == _data[type].end() )
            {
                throw std::logic_error( "No asset of given type for given key!" );                
            }

            _data[type].erase( it );
        }
};

}

}

#endif
