
#include "IEvent.hpp"
#include "IEventHandler.hpp"

#include <Sfml/System.hpp>

#include <map>
#include <deque>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include <stdexcept>
#include <algorithm>

#ifndef _EVENTMANAGER_
#define _EVENTMANAGER_

namespace BlurryRoots
{

namespace Yanecos
{

class EventManager
{
    private:

        std::map<std::type_index, std::map<std::type_index, std::vector<std::shared_ptr<IEventHandler>>>> _handlers;

        std::map<std::type_index, std::deque<std::shared_ptr<IEvent>>> _events;

        sf::Time _grantedProcessingTime;

        bool
        QueueType( std::shared_ptr<IEvent> someEvent, std::type_index someType );

        bool
        FireType( std::shared_ptr<IEvent> someEvent, std::type_index someType );

    public:

        EventManager()
            : _grantedProcessingTime( sf::milliseconds( 5 ) )
        {            
        }

        template<class EventT, class HandlerT>
        void
        AddHandler( std::shared_ptr<IEventHandler> someHandler )
        {
            static_assert(
                std::is_convertible<EventT*, IEvent*>::value,
                "Given type of event does not implement IEvent!"
            );

            static_assert(
                std::is_convertible<HandlerT*, IEventHandler*>::value,
                "Given type of handler does not implement IEventHandler!"
            );

            std::type_index eventType = typeid( EventT );
            std::type_index handlerType = typeid( HandlerT );

            if( _handlers.count( eventType ) == 0 )
            {
                _handlers[eventType] =
                    std::map<std::type_index, std::vector<std::shared_ptr<IEventHandler>>>();
            }

            if( _handlers[eventType].count( handlerType ) == 0 )
            {
                _handlers[eventType][handlerType] = 
                    std::vector<std::shared_ptr<IEventHandler>>();
            }

            auto it = std::find(
                _handlers[eventType][handlerType].begin(),
                _handlers[eventType][handlerType].end(),
                someHandler
            );
            if( it != _handlers[eventType][handlerType].end() )
            {
                throw std::logic_error(
                    "Cannot register the same handler twice!"
                );
            }

            _handlers[eventType][handlerType].push_back( someHandler );
        }

        template<class EventT, class HandlerT>
        void
        RemoveHandler( void )
        {       
            static_assert(
                std::is_convertible<EventT*, IEvent*>::value,
                "Given type of event does not implement IEvent!"
            );

            static_assert(
                std::is_convertible<HandlerT*, IEventHandler*>::value,
                "Given type of handler does not implement IEventHandler!"
            );

            std::type_index eventType = typeid( EventT );
            std::type_index handlerType = typeid( HandlerT );

            if( _handlers.count( eventType ) == 0 )
            {
                throw std::logic_error(
                    "No handlers for given type of event registered!"
                );
            }

            auto it = std::find(
                _handlers[eventType].begin(),
                _handlers[eventType].end(),
                handlerType
            );
            if( it == _handlers[eventType].end() )
            {
                throw std::logic_error(
                    "No handlers for given type registered!"
                );
            }

            _handlers[eventType].erase( it );
        }

        template<class EventT, class HandlerT>
        void
        RemoveHandler( std::shared_ptr<IEventHandler> someHandler )
        {       
            static_assert(
                std::is_convertible<EventT*, IEvent*>::value,
                "Given type of event does not implement IEvent!"
            );

            static_assert(
                std::is_convertible<HandlerT*, IEventHandler*>::value,
                "Given type of handler does not implement IEventHandler!"
            );

            std::type_index eventType = typeid( EventT );
            std::type_index handlerType = typeid( HandlerT );

            if( _handlers.count( eventType ) == 0 )
            {
                throw std::logic_error(
                    "No handlers for given type of event registered!"
                );
            }

            if( _handlers[eventType].count( handlerType ) == 0 )
            {
                throw std::logic_error(
                    "No handlers for given type registered!"
                );
            }

            auto it = std::find(
                _handlers[eventType][handlerType].begin(),
                _handlers[eventType][handlerType].end(),
                someHandler
            );
            if( it == _handlers[eventType][handlerType].end() )
            {
                throw std::logic_error(
                    "Given handler cannot be removed, because it hasnt been registered before!"
                );
            }

            _handlers[eventType][handlerType].erase( it );
        }

        template<class EventT>
        bool
        Queue( std::shared_ptr<IEvent> someEvent )
        { 
            static_assert(
                std::is_convertible<EventT*, IEvent*>::value,
                "Given type of event does not implement IEvent!"
            );

            std::type_index eventType = typeid( EventT );

            return this->QueueType( someEvent, eventType );
        }

        template<class EventT>
        bool
        Fire( std::shared_ptr<IEvent> someEvent )
        { 
            static_assert(
                std::is_convertible<EventT*, IEvent*>::value,
                "Given type of event does not implement IEvent!"
            );

            std::type_index eventType = typeid( EventT );

            return this->FireType( someEvent, eventType );
        }

        void
        GrantTime( sf::Time someTime );

        void
        Update( sf::Time someDeltaTime );
};

}

}

#endif
