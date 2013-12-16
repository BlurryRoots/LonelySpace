
#include "BaseSystem.hpp"

#include <BlurryRoots/Yanecos/AssetManager.hpp>
#include <BlurryRoots/Yanecos/EventManager.hpp>

#ifndef _LOGICSYSTEM_
#define _LOGICSYSTEM_

class LogicSystem
    : public BaseSystem
{
    protected:

        BlurryRoots::Yanecos::EventManager& _eventManager;

    public:

        LogicSystem(
            BlurryRoots::Yanecos::EventManager& someEventManager )
            : _eventManager( someEventManager )
        {
        }
};

#endif
