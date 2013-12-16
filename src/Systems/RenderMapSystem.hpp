
#include "../AssetKeys.hpp"

#include "../Data/TileData.hpp"
#include "../Data/PositionData.hpp"
#include "../Data/CameraData.hpp"

#include "RenderSystem.hpp"

#include <BlurryRoots/Yanecos/AssetManager.hpp>
#include <BlurryRoots/Yanecos/EventManager.hpp>
#include <BlurryRoots/Yanecos/EntityManager.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>

#ifndef _RENDERMAPSYSTEM_
#define _RENDERMAPSYSTEM_

class RenderMapSystem
    : public RenderSystem,
      public BlurryRoots::Yanecos::IEventHandler,
      public std::enable_shared_from_this<RenderMapSystem>
{
    private:

        bool _isReady;

        int _tileSize;

        sf::IntRect _currentTile;

        sf::Sprite _spritesheet;

    public:

        RenderMapSystem(
            BlurryRoots::Yanecos::EventManager& someEventManager,
            BlurryRoots::Yanecos::AssetManager& someAssetManager,
            BlurryRoots::Yanecos::EntityManager& someEntityManager,            
            sf::RenderTarget& someRenderTarget,
            int someTileSize )
            : RenderSystem( 
                someEventManager,
                someAssetManager,
                someEntityManager,
                someRenderTarget ),
              _tileSize( someTileSize ),
              _isReady( false )
        {
        }

        void
        OnStart()
        {
            _eventManager.AddHandler<AssetsReadyEvent, RenderMapSystem>(
                this->shared_from_this()
            );

            _currentTile.width = 32;
            _currentTile.height = 32;
            _currentTile.top = 0;
            _currentTile.left = 0;
        }

        void
        OnStop()
        {
            _eventManager.AddHandler<AssetsReadyEvent, RenderMapSystem>(
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
                _entityManager.GetEntitiesWithTag( "MAP" );
            if( ids.empty() )
            {
                _status = BlurryRoots::Yanecos::ISystem::Status::Error;
            }

            for( auto id : ids )
            {
                {
                    auto data = 
                        _entityManager.GetEntityDataOfType<PositionData>( id );

                    _spritesheet.setPosition( 
                        data->x * _tileSize,
                        data->y * _tileSize
                    );
                }
                {
                    auto data =
                        _entityManager.GetEntityDataOfType<TileData>( id );

                    /*auto texPtr =
                        _assetManager.GetAs<sf::Texture>( data->type );

                    sprite.setTexture( *texPtr );
                    sprite.setScale(
                        (float)_tileSize / (float)texPtr->getSize().x,
                        (float)_tileSize / (float)texPtr->getSize().y
                    );*/
                    if( data->type == "TopLeftCorner" )
                    {
                        _currentTile.left = 4 * 32;
                        _currentTile.top =  6 * 32;                        
                    }
                    else if( data->type == "TopWall" )
                    {
                        _currentTile.left = 5 * 32;
                        _currentTile.top =  6 * 32;
                    }
                    else if( data->type == "TopRightCorner" )
                    {
                        _currentTile.left = 6 * 32;
                        _currentTile.top =  6 * 32;
                    }
                    else if( data->type == "LeftWall" )
                    {
                        _currentTile.left =  4 * 32;
                        _currentTile.top =  10 * 32;
                    }
                    else if( data->type == "BottomLeftCorner" )
                    {
                        _currentTile.left =  4 * 32;
                        _currentTile.top =  11 * 32;
                    }
                    else if( data->type == "BottomWall" )
                    {
                        _currentTile.left =  5 * 32;
                        _currentTile.top =  11 * 32;
                    }
                    else if( data->type == "BottomRightCorner" )
                    {
                        _currentTile.left =  6 * 32;
                        _currentTile.top =  11 * 32;
                    }
                    else if( data->type == "RightWall" )
                    {
                        _currentTile.left =  6 * 32;
                        _currentTile.top =  10 * 32;
                    }
                    else if( data->type == "Floor" )
                    {
                        _currentTile.left = 5 * 32;
                        _currentTile.top =  8 * 32;
                    }
                }

                _spritesheet.setScale(
                    (float)_tileSize / 32.0f,
                    (float)_tileSize / 32.0f
                );

                _spritesheet.setTextureRect(
                    _currentTile
                );

                auto cams =
                    _entityManager.GetEntitiesWithTag( "PLAYER" );    
                if( cams.empty() )
                {
                    _status = BlurryRoots::Yanecos::ISystem::Status::Error;   
                }
                auto camera =
                    _entityManager.GetEntityDataOfType<CameraData>( cams.front() );

                _renderTarget.setView( camera->view );

                _renderTarget.draw( _spritesheet );
            }
        }

        bool
        Handle( std::shared_ptr<BlurryRoots::Yanecos::IEvent> someEvent )
        {
            /*static bool hasWallTile = false;
            static bool hasFloorTile = false;*/

            auto e =
                std::dynamic_pointer_cast<AssetsReadyEvent>( someEvent );
            if( e == nullptr )
            {
                throw std::logic_error( "Wrong type of event!" );
            }

            /*if( e->key == TileType::Wall::Brick )
            {
                hasWallTile = true;
                _isReady = hasWallTile && hasFloorTile;

                return true;
            }
            else if( e->key == TileType::Floor::Dirt )
            {
                hasFloorTile = true;   
                _isReady = hasWallTile && hasFloorTile;             

                return true;
            }*/
            if( e->key == "spritesheet" )
            {
                _isReady = true;

                _spritesheet.setTexture( 
                    *(_assetManager.GetAs<sf::Texture>( e->key )) 
                );
            }


            return false;
        }
};

#endif
