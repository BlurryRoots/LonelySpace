
#include "RenderSystem.hpp"

#include "../AssetKeys.hpp"

#include <BlurryRoots/Yanecos/AssetManager.hpp>
#include <BlurryRoots/Yanecos/EventManager.hpp>
#include <BlurryRoots/Yanecos/EntityManager.hpp>
#include <BlurryRoots/Yanecos/IEvent.hpp>
#include <BlurryRoots/Yanecos/IEventHandler.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>
#include <memory>

#ifndef _FPSSYSTEM_
#define _FPSSYSTEM_

class RenderFpsSystem
    : public RenderSystem,
      public BlurryRoots::Yanecos::IEventHandler,
      public std::enable_shared_from_this<RenderFpsSystem>
{
    private:

        bool _isReady;

    public:

        RenderFpsSystem(
            BlurryRoots::Yanecos::EventManager& someEventManager,
            BlurryRoots::Yanecos::AssetManager& someAssetManager,
            BlurryRoots::Yanecos::EntityManager& someEntityManager,            
            sf::RenderTarget& someRenderTarget )
            : RenderSystem( 
                someEventManager,
                someAssetManager,
                someEntityManager,
                someRenderTarget ),
              _isReady( false )
        {
        }

        void
        OnStart()
        {
            _eventManager.AddHandler<AssetsReadyEvent, RenderFpsSystem>(
                this->shared_from_this()
            );
        }

        void
        OnStop()
        {
            _eventManager.AddHandler<AssetsReadyEvent, RenderFpsSystem>(
                this->shared_from_this()
            );
        }

        void
        OnUpdate( sf::Time someDeltaTime )
        {
            if( ! _isReady )
            {
                return;
            }

            auto ids =
                _entityManager.GetEntitiesWithTag( "FPS" );

            if( ids.empty() )
            {
                _status = BlurryRoots::Yanecos::ISystem::Status::Error;
            }

            std::size_t id = ids.front();

            sf::Text t;
            {
                auto data = 
                    _entityManager.GetEntityDataOfType<PositionData>( id );
                t.setPosition( data->x, data->y );
            }
            {
                auto data = 
                    _entityManager.GetEntityDataOfType<TextData>( id );

                std::stringstream ss;
                ss << (1.0 / someDeltaTime.asSeconds());
                data->text = ss.str();

                t.setFont( *(data->font) );
                t.setString( "FPS: " + data->text );
                t.setColor( data->color );
            }

            _renderTarget.draw( t );
            
        }

        bool
        Handle( std::shared_ptr<BlurryRoots::Yanecos::IEvent> someEvent )
        {
            auto e =
                std::dynamic_pointer_cast<AssetsReadyEvent>( someEvent );

            if( e->key == AssetKeys::DefaultFont )
            {
                std::size_t id =
                    _entityManager.GetEntitiesWithTag( "FPS" ).front();

                auto data = 
                    _entityManager.GetEntityDataOfType<TextData>( id );

                data->font =
                    _assetManager.GetAs<sf::Font>( e->key );

                _isReady = true;

                return true;
            }

            return false;
        }
};

#endif
