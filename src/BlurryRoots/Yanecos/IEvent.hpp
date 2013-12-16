
#include <typeinfo>
#include <typeindex>

#ifndef _IEVENT_
#define _IEVENT_

namespace BlurryRoots
{

namespace Yanecos
{

class IEvent
{
    public:

        virtual
            ~IEvent() {}

        virtual
            std::type_index
            GetType() = 0;
};

}

}

#endif
