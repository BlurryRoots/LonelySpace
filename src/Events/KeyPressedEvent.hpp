
#include <BlurryRoots/Yanecos/IEvent.hpp>

#include <SFML/System.hpp>

#include <typeinfo>
#include <typeindex>

#ifndef _KEYPRESSEDEVENT_
#define _KEYPRESSEDEVENT_

struct KeyPressedEvent
    : public BlurryRoots::Yanecos::IEvent
{
    sf::Event::KeyEvent data;
    
    std::type_index
    GetType()
    {
        return typeid( KeyPressedEvent );
    }
};

#endif
