
#include <BlurryRoots/Yanecos/IEvent.hpp>

#include <typeinfo>
#include <typeindex>

#include <string>

#ifndef _ASSETSREADYEVENT_
#define _ASSETSREADYEVENT_

class AssetsReadyEvent
    : public BlurryRoots::Yanecos::IEvent
{
    private:
    public:

        AssetsReadyEvent( std::string someKey )
            : key( someKey )
        {}

        // key of asset successfully loaded
        std::string key;

        std::type_index
        GetType()
        {
            return typeid( AssetsReadyEvent );
        }
};

#endif
