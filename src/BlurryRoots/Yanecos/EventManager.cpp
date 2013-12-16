
#include "EventManager.hpp"

#include <iostream>

namespace BlurryRoots
{

namespace Yanecos
{

bool
EventManager::QueueType( std::shared_ptr<IEvent> someEvent, std::type_index someType )
{
    if( _handlers.count( someType ) == 0 )
    {
        // event has not been queued, because there
        // is nobody whos interested in it
        return false;
    }

    if( _events.count( someType ) == 0 )
    {
        // create new container for this type of event
        _events[someType] = 
            std::deque<std::shared_ptr<IEvent>>();
    }

    auto it = std::find(
        _events[someType].begin(),
        _events[someType].end(),
        someEvent
    );
    if( it != _events[someType].end() )
    {
        // oops this event has already been queued!
        throw std::logic_error(
            "Given event has already been queued!"
        );
    }

    _events[someType].push_back( someEvent );

    return true;
}

bool
EventManager::FireType( std::shared_ptr<IEvent> someEvent, std::type_index someType )
{
    if( _handlers.count( someType ) == 0 )
    {
        // theres nobody to liste to this event :(
        return false;
    }

    for( auto entry : _handlers[someType] )
    {
        for( auto handler : entry.second )
        {
            if( handler->Handle( someEvent ) )
            {
                // event has been consumed
                // call it a day
                return true;
            }
        }
    }

    // nobody consumed this event oO
    return false;

}

void
EventManager::GrantTime( sf::Time someTime )
{
    _grantedProcessingTime = someTime;
}

void
EventManager::Update( sf::Time someDeltaTime )
{    
    if( _events.empty() )
    {
        return;
    }

    sf::Clock diffTimeClock;
    sf::Time timeAccu = sf::seconds( 0 );

    std::size_t amount = 0;
    for( auto entry : _events )
    {
        amount += entry.second.size();
    }

    // buffer events because handling events can spawn new events
    auto eventsBuffer =
        std::map<std::type_index, std::deque<std::shared_ptr<IEvent>>>(
            _events.begin(),
            _events.end()
        );
    // clear all events
    _events.clear();

    // handle events
    bool timeIsUp = false;
    for( auto& entry : eventsBuffer )
    {
        auto type = entry.first;
        auto& queue = entry.second;

        if( _handlers.count( type ) == 0 )
        {
            // noone listens to this type of event
            std::cout 
                << "Clearing events of type " << type.name() << "\n"
                << "Noone seems to be interested!\n";
            queue.clear();  // remove all events for this type
            continue;       // skip this type
        }

        while( ! queue.empty() )
        {
            timeAccu += diffTimeClock.restart();
            if( timeAccu >= _grantedProcessingTime )
            {
                timeIsUp = true;
                break;
            }

            auto event = queue.front();
            queue.pop_front();
            bool hasBeenHandled = false;
            for( auto handlerMap : _handlers[type] )
            {
                for( auto handler : handlerMap.second )
                {
                    if( handler->Handle( event ) )
                    {
                        hasBeenHandled = true;
                        break;
                    }
                }

                if( hasBeenHandled )
                {
                    break;
                }
            }
        }

        if( timeIsUp )
        {
            break;
        }
    }

    // reverse buffer and requeue remaining events
    for( auto entry : eventsBuffer )
    {
        auto type = entry.first;
        auto queue = entry.second;

        if( queue.empty() )
        {
            // if there are no events of current
            // type, skip this queue
            continue;
        }

        if( _events.count( type ) == 0 )
        {
            _events[type] =
                std::deque<std::shared_ptr<IEvent>>();
        }

        while( ! queue.empty() )
        {
            _events[type].push_front( queue.back() );
            queue.pop_back();
        }
    }
}

}

}
