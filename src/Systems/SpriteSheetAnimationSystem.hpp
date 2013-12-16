
#include "../AssetKeys.hpp"

#include "../Data/PositionData.hpp"
#include "../Data/RotationData.hpp"
#include "../Data/SpriteSheetAnimationData.hpp"

#include "RenderSystem.hpp"

#include <BlurryRoots/Yanecos/AssetManager.hpp>
#include <BlurryRoots/Yanecos/EventManager.hpp>
#include <BlurryRoots/Yanecos/EntityManager.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <string>
#include <vector>

#ifndef _SPRITESHEETANIMATIONSYSTEM_
#define _SPRITESHEETANIMATIONSYSTEM_

class SpriteSheetAnimationSystem
    : public RenderSystem,
      public BlurryRoots::Yanecos::IEventHandler,
      public std::enable_shared_from_this<SpriteSheetAnimationSystem>
{
    private:

        bool _isReady;

        std::size_t _tileSize;

        sf::Sprite _spritesheet;

        std::vector<std::size_t> _dataRequest;

    public:

        SpriteSheetAnimationSystem(
            BlurryRoots::Yanecos::EventManager& someEventManager,
            BlurryRoots::Yanecos::AssetManager& someAssetManager,
            BlurryRoots::Yanecos::EntityManager& someEntityManager,            
            sf::RenderTarget& someRenderTarget,
            std::size_t someTileSize )
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
            _eventManager.AddHandler<AssetsReadyEvent, SpriteSheetAnimationSystem>(
                this->shared_from_this()
            );

            _dataRequest = std::vector<std::size_t>{
                typeid( SpriteSheetAnimationData ).hash_code(),
                typeid( PositionData ).hash_code(),
                typeid( RotationData ).hash_code()
            };
        }

        void
        OnStop()
        {
            _eventManager.AddHandler<AssetsReadyEvent, SpriteSheetAnimationSystem>(
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

            auto ids = _entityManager.GetEntitiesWithData(
                _dataRequest
            );

            if( ids.empty() )
            {
                _status = BlurryRoots::Yanecos::ISystem::Status::Error;
            }

            for( auto id : ids )
            {
                {
                    auto data =
                        _entityManager.GetEntityDataOfType<SpriteSheetAnimationData>( id );

                    auto texPtr =
                        _assetManager.GetAs<sf::Texture>( data->sheetName );
                    if( _spritesheet.getTexture() != texPtr.get() )
                    {
                        _spritesheet.setTexture( *texPtr );
                    }

                    sf::IntRect rect = data->cell;
                    rect.top = data->cell.top * data->cell.height;
                    rect.left = data->cell.left * data->cell.width;

                    _spritesheet.setTextureRect(
                        rect
                    );

                    _spritesheet.setScale(
                        (float)_tileSize / (float)data->cell.width,
                        (float)_tileSize / (float)data->cell.height
                    );
                }
                {
                    auto rotation = 
                        _entityManager.GetEntityDataOfType<RotationData>( id );

                    _spritesheet.setRotation( rotation->degrees );

                    auto pos = 
                        _entityManager.GetEntityDataOfType<PositionData>( id );

                    float x = pos->x * _tileSize;
                    float y = pos->y * _tileSize;

                    if( _spritesheet.getRotation() == 90 )
                    {
                        x += _tileSize;
                    }
                    else if( _spritesheet.getRotation() == 180 )
                    {
                        x += _tileSize;
                        y += _tileSize;
                    }
                    else if( _spritesheet.getRotation() == 270 )
                    {
                        y += _tileSize;                        
                    }

                    _spritesheet.setPosition( x, y );
                }

                _renderTarget.draw( _spritesheet );
            }
        }

        bool
        Handle( std::shared_ptr<BlurryRoots::Yanecos::IEvent> someEvent )
        {
            auto e =
                std::dynamic_pointer_cast<AssetsReadyEvent>( someEvent );
            if( e == nullptr )
            {
                throw std::logic_error( "Wrong type of event!" );
            }

            if( e->key == "spritesheet" )
            {
                std::cout << "SpriteSheetAnimationSystem is ready!\n";

                _isReady = true;

                _spritesheet.setTexture( 
                    *(_assetManager.GetAs<sf::Texture>( e->key )) 
                );
            }


            return false;
        }
};

#endif
