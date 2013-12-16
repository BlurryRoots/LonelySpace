
#include "IProcess.hpp"

#include <SFML/System.hpp>

#include <memory>
#include <deque>
#include <iostream>
#include <stdexcept>

#ifndef _PROCESSMANAGER_
#define _PROCESSMANAGER_

namespace BlurryRoots
{

namespace Yanecos
{

class ProcessManager
{
    private:

        std::deque<std::shared_ptr<IProcess>> _processes;

        sf::Time _grantedProcessingTime;

    public:

        ProcessManager()
            : _grantedProcessingTime( sf::milliseconds( 5 ) ) // 0.016s is 60Hz so 11ms are left for other stuff
        {            
            std::cout << "ProcessManager created!\n";
        }

        ~ProcessManager()
        {
            std::cout << "ProcessManager destroyed!\n";
        }

        template<class ProcessT>
        std::shared_ptr<ProcessT>
        Add( std::shared_ptr<ProcessT> someProcess )
        {
            static_assert(
                std::is_convertible<ProcessT*, IProcess*>::value,
                "Given type of process does not implement IProcess!"
            );

            if( someProcess == nullptr )
            {
                throw std::logic_error(
                    "Trying to add invalid process pointer!"
                );
            }

            _processes.push_back( someProcess );

            return someProcess;            
        }

        void
        Update( sf::Time someDeltaTime );

        void
        GrantTime( sf::Time someTime );
};

}

}

#endif
