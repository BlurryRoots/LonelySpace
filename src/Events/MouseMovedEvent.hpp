
#include <BlurryRoots/Yanecos/IEvent.hpp>

#include <SFML/System.hpp>

#include <typeinfo>
#include <typeindex>

#ifndef _MOUSEMOVEDEVENT_
#define _MOUSEMOVEDEVENT_

struct MouseMovedEvent
    : public BlurryRoots::Yanecos::IEvent
{
    sf::Vector2f previousPosition;
    sf::Vector2f currentPosition;

    std::type_index
    GetType()
    {
        return typeid( MouseMovedEvent );
    }
};

#endif
