
#include <Sfml/System.hpp>

#ifndef _IPROCESS_
#define _IPROCESS_

namespace BlurryRoots
{

namespace Yanecos
{

class IProcess
{
    public:

        enum Status
        {
            Stopped,
            Running,
            Error,
            Done
        };

        virtual
            ~IProcess() {}

        virtual
            void
            Update( sf::Time someDeltaTime ) = 0;
            
        virtual
            IProcess::Status
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
