
#include "ProcessManager.hpp"

namespace BlurryRoots
{

namespace Yanecos
{

void
ProcessManager::Update( sf::Time someDeltaTime )
{
    sf::Clock diffTimeClock;
    sf::Time timeAccu = sf::seconds( 0 );

    while( ! _processes.empty() )
    {
        timeAccu += diffTimeClock.restart();
        if( timeAccu >= _grantedProcessingTime )
        {
            break;
        }

        auto current = _processes.front();
        _processes.pop_front();
        
        switch( current->GetStatus() )
        {
            case IProcess::Status::Stopped:
            {
                current->Start();

                _processes.push_back( current );
            }
            break;

            case IProcess::Status::Running:
            {
                current->Update( someDeltaTime + timeAccu );

                _processes.push_back( current );
            }
            break;

            case IProcess::Status::Done:
            {
                current->Stop();

                // report that process is done !
            }
            break;

            case IProcess::Status::Error:
            {
                throw std::logic_error( "Process Error!" );
            }
            break;
        }
    }
}

void
ProcessManager::GrantTime( sf::Time someTime )
{
    _grantedProcessingTime = someTime;
}

}

}
