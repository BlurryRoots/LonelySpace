
#include "ISystem.hpp"

#include <SFML/System.hpp>

#include <typeinfo>
#include <typeindex>
#include <memory>
#include <deque>
#include <iostream>
#include <stdexcept>

#ifndef _SYSTEMMANAGER_
#define _SYSTEMMANAGER_

namespace BlurryRoots
{

namespace Yanecos
{

class SystemManager
{
    private:

        std::map<std::type_index, std::shared_ptr<ISystem>> _systems;

        sf::Time _grantedProcessingTime;

    public:

        SystemManager()
            : _grantedProcessingTime( sf::milliseconds( 5 ) )
        {            
        }

        ~SystemManager()
        {
        }

        template<class SystemT>
        std::shared_ptr<SystemT>
        Add( std::shared_ptr<SystemT> someSystem )
        {
            static_assert(
                std::is_convertible<SystemT*, ISystem*>::value,
                "Given type of system does not implement ISystem!"
            );

            if( someSystem == nullptr )
            {
                throw std::logic_error(
                    "Trying to add invalid system pointer!"
                );
            }

            std::type_index type = typeid( SystemT );
            
            if( _systems.count( type ) > 0 )
            {
                throw std::logic_error(
                    "Cannot add the same system twice!"
                );
            }

            _systems[type] = someSystem;

            someSystem->Start();

            return someSystem;            
        }

        template<class SystemT>
        std::shared_ptr<SystemT>
        Get()
        {
            static_assert(
                std::is_convertible<SystemT*, ISystem*>::value,
                "Given type of system does not implement ISystem!"
            );

            std::type_index type = typeid( SystemT );

            if( _systems.count( type ) == 0 )
            {
                throw std::logic_error(
                    "No system with given type registered!"
                );
            }

            return std::dynamic_pointer_cast<SystemT>( _systems[type] );
        }

        template<class SystemT>
        std::shared_ptr<SystemT>
        Remove()
        {
            static_assert(
                std::is_convertible<SystemT*, ISystem*>::value,
                "Given type of system does not implement ISystem!"
            );

            std::type_index type = typeid( SystemT );

            if( _systems.count( type ) == 0 )
            {
                throw std::logic_error(
                    "No system with given type registered!"
                );
            }

            auto it = _systems.find( type );

            it->second->Stop();

            _systems.erase( it );
        }

        void
        GrantTime( sf::Time someTime )
        {
            _grantedProcessingTime = someTime;
        }
};

}

}

#endif
