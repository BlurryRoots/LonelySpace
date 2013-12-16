
#include <Sfml/System.hpp>

#ifndef _ISYSTEM_
#define _ISYSTEM_

namespace BlurryRoots
{

namespace Yanecos
{

class ISystem
{
    public:

        enum Status
        {
            Stopped,
            Running,
            Error
        };

        virtual
            ~ISystem() {}

        virtual
            void
            Update( sf::Time someDeltaTime ) = 0;
            
        virtual
            ISystem::Status
            GetStatus() = 0;

        virtual
            void
            Start() = 0;

        virtual
            void
            OnStart() = 0;

        virtual
            void
            Stop() = 0;

        virtual
            void
            OnStop() = 0;
};

}

}

#endif
