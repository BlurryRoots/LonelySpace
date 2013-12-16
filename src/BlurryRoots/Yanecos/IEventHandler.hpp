
#include "IEvent.hpp"

#include <memory>

#ifndef _IEVENTHANDLER_
#define _IEVENTHANDLER_

namespace BlurryRoots
{

namespace Yanecos
{

class IEventHandler
{
    public:

        virtual
            ~IEventHandler() {}

        virtual
            bool
            Handle( std::shared_ptr<IEvent> someEvent ) = 0;
};

}

}

#endif
