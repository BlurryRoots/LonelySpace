
#include "../Data/PositionData.hpp"
#include "../Data/TileData.hpp"

#include "LogicSystem.hpp"

#include <BlurryRoots/Yanecos/EventManager.hpp>
#include <BlurryRoots/Yanecos/EntityManager.hpp>
#include <BlurryRoots/Yanecos/IEventHandler.hpp>
#include <BlurryRoots/Yanecos/IEvent.hpp>

#include <memory>
#include <deque>

#ifndef _ACTORMOVESYSTEM_
#define _ACTORMOVESYSTEM_

class ActorMoveSystem
    : public LogicSystem,
      public BlurryRoots::Yanecos::IEventHandler,
      public std::enable_shared_from_this<ActorMoveSystem>

{
    protected:

        BlurryRoots::Yanecos::EntityManager& _entityManager;

        std::deque<std::shared_ptr<ActorMoveRequestEvent>> _moveRequests;

    public:

        ActorMoveSystem(
            BlurryRoots::Yanecos::EventManager& someEventManager,
            BlurryRoots::Yanecos::EntityManager& someEntityManager )
            : LogicSystem( someEventManager ),
              _entityManager( someEntityManager )
        {
        }

        void
        OnStart()
        {
            _eventManager.AddHandler<ActorMoveRequestEvent, ActorMoveSystem>(
                this->shared_from_this()
            );
        }

        void
        OnStop()
        {
            _eventManager.RemoveHandler<ActorMoveRequestEvent, ActorMoveSystem>(
                this->shared_from_this()
            );
        }

        void
        OnUpdate( sf::Time someDeltaTime )
        {            
            auto mapIds =
                _entityManager.GetEntitiesWithTag( "MAP" );

            auto requests =
                std::deque<std::shared_ptr<ActorMoveRequestEvent>>(
                    _moveRequests.begin(),
                    _moveRequests.end()
                );
            _moveRequests.clear();

            while( ! requests.empty() )
            {
                auto request = requests.front();
                requests.pop_front();

                if( request->rotate )
                {
                    auto rotation = 
                        _entityManager
                            .GetEntityDataOfType<RotationData>(
                                request->id 
                        );
                    rotation->degrees = request->angle;
                }

                if( request->move )
                {
                    auto pos = 
                        _entityManager
                            .GetEntityDataOfType<PositionData>(
                                request->id 
                        );
                    for( auto tileId : mapIds )
                    {
                        auto tilePos =
                            _entityManager
                                .GetEntityDataOfType<PositionData>(
                                    tileId
                            );

                        if( tilePos->x == (pos->x + request->direction.x)
                         && tilePos->y == (pos->y + request->direction.y) )
                        {                        
                            auto tileData =
                                _entityManager
                                    .GetEntityDataOfType<TileData>(
                                        tileId
                                );
                            if( tileData->accessible )
                            {
                                std::cout
                                    << "moving " 
                                    << _entityManager.GetEntityTag( request->id )
                                    << " " << request->direction.x 
                                    << ":" << request->direction.y << "\n";
                                    
                                pos->x += request->direction.x;
                                pos->y += request->direction.y; 

                                break;
                            }
                        }
                    }
                }
            }
        }

        bool
        Handle( std::shared_ptr<BlurryRoots::Yanecos::IEvent> someEvent )
        {
            if( someEvent->GetType() == typeid( ActorMoveRequestEvent ) )
            {
                auto event =
                    std::static_pointer_cast<ActorMoveRequestEvent>(
                        someEvent 
                    );

                _moveRequests.push_back( event );
            }
        }
};

#endif
