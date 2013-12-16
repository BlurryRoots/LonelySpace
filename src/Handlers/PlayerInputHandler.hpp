
#include "../Events/KeyPressedEvent.hpp"
#include "../Events/ActorMoveRequestEvent.hpp"

#include "../Data/PositionData.hpp"
#include "../Data/SpriteSheetAnimationData.hpp"

#include <BlurryRoots/Yanecos/EventManager.hpp>
#include <BlurryRoots/Yanecos/EntityManager.hpp>

#include <iostream>

#ifndef _PLAYERKEYBOARDINPUTHANDLER_
#define _PLAYERKEYBOARDINPUTHANDLER_

class PlayerInputHandler
    : public BlurryRoots::Yanecos::IEventHandler,
      public std::enable_shared_from_this<PlayerInputHandler>
{
    private:

        BlurryRoots::Yanecos::EventManager& _eventManager;

        BlurryRoots::Yanecos::EntityManager& _entityManager;

        bool _targetEnabled;

    public:

        PlayerInputHandler(
            BlurryRoots::Yanecos::EventManager& someEventManager,
            BlurryRoots::Yanecos::EntityManager& someEntityManager )
            : _eventManager( someEventManager ),
              _entityManager( someEntityManager ),
              _targetEnabled( false )
        {}

        void
        HandleMoving( std::shared_ptr<KeyPressedEvent> someEvent )
        {
            auto event = 
                std::make_shared<ActorMoveRequestEvent>();
            event->id =
                _entityManager.GetEntitiesWithTag( "PLAYER" ).front();
            event->move = ! someEvent->data.shift;
            event->rotate = true;

            if( someEvent->data.code == sf::Keyboard::Key::W )
            {
                event->direction.x =  0;
                event->direction.y = -1;
                
                event->angle = 0;
            }
            else if( someEvent->data.code == sf::Keyboard::Key::A )
            {
                event->direction.x = -1;
                event->direction.y =  0;

                event->angle = 270;
            }
            else if( someEvent->data.code == sf::Keyboard::Key::S )
            {
                event->direction.x =  0;
                event->direction.y =  1;

                event->angle = 180;
            }
            else if( someEvent->data.code == sf::Keyboard::Key::D )
            {
                event->direction.x =  1;
                event->direction.y =  0;
                
                event->angle = 90;
            }

            _eventManager.Queue<ActorMoveRequestEvent>(
                event
            );
        }

        void
        HandleTargeting( std::shared_ptr<KeyPressedEvent> someEvent )
        {
            auto event = 
                std::make_shared<ActorMoveRequestEvent>();
            event->id =
                _entityManager.GetEntitiesWithTag( "TARGET" ).front();

            event->move = true;
            event->rotate = false;

            if( someEvent->data.code == sf::Keyboard::Key::Up )
            {
                event->direction.x =  0;
                event->direction.y = -1;
            }
            else if( someEvent->data.code == sf::Keyboard::Key::Left )
            {
                event->direction.x = -1;
                event->direction.y =  0;
            }
            else if( someEvent->data.code == sf::Keyboard::Key::Down )
            {
                event->direction.x =  0;
                event->direction.y =  1;     
            }
            else if( someEvent->data.code == sf::Keyboard::Key::Right )
            {
                event->direction.x =  1;
                event->direction.y =  0;
            }

            _eventManager.Queue<ActorMoveRequestEvent>(
                event
            );
        }

        void
        DisableTargeting()
        {
            _targetEnabled = false;

            auto targetId =
                _entityManager.GetEntitiesWithTag( "TARGET" ).front();

            auto anim =
                _entityManager.GetEntityDataOfType<SpriteSheetAnimationData>( targetId );
            anim->cell.top = 2;
        }

        void
        EnableTargeting()
        {
            _targetEnabled = true;

            auto playerId =
                _entityManager.GetEntitiesWithTag( "PLAYER" ).front();
            auto targetId =
                _entityManager.GetEntitiesWithTag( "TARGET" ).front();  

            auto anim =
                _entityManager.GetEntityDataOfType<SpriteSheetAnimationData>( targetId );
            anim->cell.top = 1;
                          
            auto playerPos =
                _entityManager.GetEntityDataOfType<PositionData>( playerId );
            auto targetPos =
                _entityManager.GetEntityDataOfType<PositionData>( targetId );

            targetPos->x = playerPos->x;
            targetPos->y = playerPos->y;
        }

        void
        HandleShooting()
        {
            auto playerId =
                _entityManager.GetEntitiesWithTag( "PLAYER" ).front();

            auto anim =
                _entityManager.GetEntityDataOfType<SpriteSheetAnimationData>( playerId );

            anim->cell.top =  0;
            anim->cell.left = 0;
        }

        bool
        Handle( std::shared_ptr<BlurryRoots::Yanecos::IEvent> someEvent )
        {
            if( someEvent->GetType() == typeid( KeyPressedEvent ) )
            {
                auto keyPressedEvent =
                    std::static_pointer_cast<KeyPressedEvent>(
                        someEvent 
                    );

                if( keyPressedEvent->data.code == sf::Keyboard::Key::W 
                 || keyPressedEvent->data.code == sf::Keyboard::Key::A 
                 || keyPressedEvent->data.code == sf::Keyboard::Key::S
                 || keyPressedEvent->data.code == sf::Keyboard::Key::D )
                {
                    if( _targetEnabled )
                    {
                        this->DisableTargeting();
                    }

                    this->HandleMoving( keyPressedEvent );
                }
                else if( keyPressedEvent->data.code == sf::Keyboard::Key::Up 
                      || keyPressedEvent->data.code == sf::Keyboard::Key::Left
                      || keyPressedEvent->data.code == sf::Keyboard::Key::Down
                      || keyPressedEvent->data.code == sf::Keyboard::Key::Right )
                {
                    if( ! _targetEnabled )
                    {
                        this->EnableTargeting();
                    }
                    this->HandleTargeting( keyPressedEvent );
                }
                else if( keyPressedEvent->data.code == sf::Keyboard::Key::Space )
                {   
                    if( _targetEnabled )
                    {
                        // Shoot
                        this->HandleShooting();
                    }
                }
                else
                {
                    this->DisableTargeting();                    
                }
            }
        }
};

#endif
