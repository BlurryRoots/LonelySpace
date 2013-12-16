
#include <BlurryRoots/Yanecos/IProcess.hpp>
#include <BlurryRoots/Yanecos/AssetManager.hpp>
#include <BlurryRoots/Yanecos/EventManager.hpp>

#ifndef _BASEPROCESS_
#define _BASEPROCESS_

class BaseProcess
    : public BlurryRoots::Yanecos::IProcess
{
    protected:

        BlurryRoots::Yanecos::EventManager& _eventManager;

        BlurryRoots::Yanecos::IProcess::Status _status;

    public:

        BaseProcess(
            BlurryRoots::Yanecos::EventManager& someEventManager )
            : _eventManager( someEventManager ),
              _status( BlurryRoots::Yanecos::IProcess::Status::Stopped )
        {
        }
            
        BlurryRoots::Yanecos::IProcess::Status
        GetStatus()
        {
            return _status;
        }

        void
        Start()
        {
            _status = BlurryRoots::Yanecos::IProcess::Running;
            this->OnStart();
        }

        void
        OnStart()
        {
        }

        void
        Stop()
        {
            _status = BlurryRoots::Yanecos::IProcess::Stopped;            
            this->OnStop();
        }

        void
        OnStop()
        {
        }
};

#endif
