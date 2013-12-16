
#include <BlurryRoots/Yanecos/ISystem.hpp>

#include <SFML/System.hpp>

#ifndef _BASESYSTEM_
#define _BASESYSTEM_

class BaseSystem
    : public BlurryRoots::Yanecos::ISystem
{
    protected:

        BlurryRoots::Yanecos::ISystem::Status _status;

    public:

        BaseSystem()
            : _status( BlurryRoots::Yanecos::ISystem::Status::Stopped )
        {}
            
        BlurryRoots::Yanecos::ISystem::Status
        GetStatus()
        {
            return _status;
        }

        void
        Start()
        {
            switch( _status )
            {
                case BlurryRoots::Yanecos::ISystem::Status::Stopped:
                {
                    _status = BlurryRoots::Yanecos::ISystem::Running;
                    this->OnStart();
                }
                break;

                case BlurryRoots::Yanecos::ISystem::Status::Running:
                {}
                break;

                case BlurryRoots::Yanecos::ISystem::Status::Error:
                {
                    throw std::logic_error( "Cannot start system with error state!" );
                }
                break;
            }
        }

        void
        OnStart()
        {
        }

        void
        Stop()
        {
            switch( _status )
            {
                case BlurryRoots::Yanecos::ISystem::Status::Stopped:
                {}
                break;

                case BlurryRoots::Yanecos::ISystem::Status::Running:
                {
                    _status = BlurryRoots::Yanecos::ISystem::Stopped;            
                    this->OnStop();
                }
                break;

                case BlurryRoots::Yanecos::ISystem::Status::Error:
                {
                    throw std::logic_error( "Cannot stop system with error state!" );
                }
                break;
            }
        }

        void
        OnStop()
        {
        }

        void
        Update( sf::Time someDeltaTime )
        {
            switch( _status )
            {
                case BlurryRoots::Yanecos::ISystem::Status::Running:
                {
                    this->OnUpdate( someDeltaTime );
                }
                break;

                case BlurryRoots::Yanecos::ISystem::Status::Error:
                {
                    throw std::logic_error( "Cannot update system with error state!" );
                }
                break;
            }
        }

        virtual
            void
            OnUpdate( sf::Time someDeltaTime ) = 0;
};

#endif
