
#include <BlurryRoots/Yanecos/IEvent.hpp>

#include <SFML/System.hpp>

#include <typeinfo>
#include <typeindex>

#ifndef _ACTORMOVEREQUESTEVENT_
#define _ACTORMOVEREQUESTEVENT_

struct ActorMoveRequestEvent
    : public BlurryRoots::Yanecos::IEvent
{
    std::size_t id;
    
    bool move;
    sf::Vector2f direction;

    bool rotate;
    std::size_t angle;
    
    std::type_index
    GetType()
    {
        return typeid( ActorMoveRequestEvent );
    }
};

#endif
