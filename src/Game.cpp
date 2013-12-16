
#include "Game.hpp"

#include "AssetKeys.hpp"

#include "Data/PositionData.hpp"
#include "Data/RotationData.hpp"
#include "Data/TextData.hpp"
#include "Data/TileData.hpp"
#include "Data/SpriteSheetAnimationData.hpp"
#include "Data/CameraData.hpp"
#include "Data/TargetData.hpp"

#include "Events/MouseMovedEvent.hpp"
#include "Events/KeyPressedEvent.hpp"

#include "Systems/RenderFpsSystem.hpp"
#include "Systems/RenderMapSystem.hpp"
#include "Systems/SpriteSheetAnimationSystem.hpp"
#include "Systems/ActorMoveSystem.hpp"

//
static const size_t TileSize = 64;

void
Game::SpawnEnemy( std::size_t someX, std::size_t someY )
{
    std::size_t id = 
        _entityManager.CreateEntity( "ENEMY" );
    {
        auto pos  = 
            _entityManager.AddDataToEntity<PositionData>( id );

        pos->x = someX;
        pos->y = someY;
    }
    {
        auto data  = 
            _entityManager.AddDataToEntity<RotationData>( id );

        data->degrees = 0;
    }
    {
        auto data  = 
            _entityManager.AddDataToEntity<SpriteSheetAnimationData>( id );

        data->frameIndex = 0;

        data->frameCount = 1;

        data->duration = sf::seconds( 0.2f );

        data->waited = sf::seconds( 0.0f );

        data->start.width = 32;
        data->start.height = 32;
        data->start.top = 0;
        data->start.left = 1;

        data->cell = data->start;

        data->sheetName = "spritesheet";
    }
}

/*SfmlGame*/
void
Game::OnInitialze()
{
    _playerInputHandler = 
        std::make_shared<PlayerInputHandler>(
            _eventManager,
            _entityManager
        );

    _eventManager.AddHandler<KeyPressedEvent, PlayerInputHandler>(
        _playerInputHandler
    );

    /**/
    this->QueueAssetLoading<sf::Font>(
        "assets/fonts/OpenSans-Regular.ttf",
        AssetKeys::DefaultFont
    );
    this->QueueAssetLoading<sf::Texture>(
        "assets/gfx/texture/spritesheet.png",
        "spritesheet"
    );
    /**/

    /* 
        add processes
    */
    // logic
    _systems.Add(
        std::make_shared<ActorMoveSystem>(
            _eventManager,
            _entityManager
        )
    );

    // rendering
    _systems.Add(
        std::make_shared<SpriteSheetAnimationSystem>(
            _eventManager,
            _assetManager,
            _entityManager,
            *( this->getRenderWindow() ),
            TileSize
        )
    ); 
    _systems.Add(
        std::make_shared<RenderMapSystem>(
            _eventManager,
            _assetManager,
            _entityManager,
            *( this->getRenderWindow() ),
            TileSize
        )
    ); 
    _systems.Add(
        std::make_shared<RenderFpsSystem>(
            _eventManager,
            _assetManager,
            _entityManager,
            *( this->getRenderWindow() )
        )
    );   
    /* 
        add processes
    */

    // create FPS entity
    {
        std::size_t id = 
            _entityManager.CreateEntity( "FPS" );
        {
            auto data  = 
                _entityManager.AddDataToEntity<PositionData>( id );

            data->x = 42;
            data->y = 42;
        }
        {
            auto data  = 
                _entityManager.AddDataToEntity<TextData>( id );
                
            data->text = "FPS: ";
            data->color = sf::Color::Yellow;
            data->font = nullptr; 
        }
    }

    // create player
    {
        std::size_t id = 
            _entityManager.CreateEntity( "PLAYER" );
        {
            auto pos  = 
                _entityManager.AddDataToEntity<PositionData>( id );

            pos->x = 1;
            pos->y = 1;

            auto cam  = 
                _entityManager.AddDataToEntity<CameraData>( id );

            cam->view = this->getRenderWindow()->getDefaultView();    
            cam->view.setCenter(
                pos->x * TileSize + (TileSize / 2.0f),
                pos->y * TileSize + (TileSize / 2.0f)
            ); 
        }
        {
            auto data  = 
                _entityManager.AddDataToEntity<RotationData>( id );

            data->degrees = 0;
        }
        {
            auto data  = 
                _entityManager.AddDataToEntity<SpriteSheetAnimationData>( id );

            data->frameIndex = 0;

            data->frameCount = 5;

            data->duration = sf::seconds( 0.2f );

            data->waited = sf::seconds( 0.0f );

            data->start.width = 32;
            data->start.height = 32;
            data->start.top = 0;
            data->start.left = 2;

            data->cell = data->start;

            data->sheetName = "spritesheet";
        }
    }

    // player targeting system
    {
        std::size_t id = 
            _entityManager.CreateEntity( "TARGET" );
        {
            auto pos  = 
                _entityManager.AddDataToEntity<PositionData>( id );

            pos->x = 1;
            pos->y = 1;
        }
        {
            auto data  = 
                _entityManager.AddDataToEntity<RotationData>( id );

            data->degrees = 0;
        }
        {
            auto data  = 
                _entityManager.AddDataToEntity<SpriteSheetAnimationData>( id );

            data->frameIndex = 0;

            data->frameCount = 1;

            data->duration = sf::seconds( 0.2f );

            data->waited = sf::seconds( 0.0f );

            data->start.width = 32;
            data->start.height = 32;
            data->start.top = 1;
            data->start.left = 9;

            data->cell = data->start;

            data->sheetName = "spritesheet";
        }
    }

    this->SpawnEnemy( 5, 3 );
    this->SpawnEnemy( 3, 5 );

    _mapCreator.CreateRoom( 
        12, 8,
         0, 0
    );
}

void
Game::OnError( std::string someError )
{
    std::cout << someError;
}

void
Game::OnClosed()
{
    std::cout << "About to close!\n";

    this->Quit();
}

void
Game::OnMouseMoved( sf::Event::MouseMoveEvent someEvent )
{
    auto e =
        std::make_shared<MouseMovedEvent>();
    e->previousPosition = _currentMousePosition;
    // do all this stuff, to ensure proper position mapping
    // when screen size is changed
    _currentMousePosition = this->getRenderWindow()->mapPixelToCoords(
        sf::Mouse::getPosition(
            *( this->getRenderWindow() )
        )
    );
    e->currentPosition = sf::Vector2f(
        _currentMousePosition.x,
        _currentMousePosition.y
    );

    _eventManager.Queue<MouseMovedEvent>( e );
}

void
Game::OnMouseButtonPressed( sf::Event::MouseButtonEvent someEvent )
{
    if( someEvent.button == sf::Mouse::Left )
    {
    }
}

void
Game::OnMouseButtonReleased( sf::Event::MouseButtonEvent someEvent )
{
    if( someEvent.button == sf::Mouse::Left )
    {
    }
}

void
Game::OnKeyPressed( sf::Event::KeyEvent someEvent )
{
    auto event =
        std::make_shared<KeyPressedEvent>();
    event->data = someEvent;

    _eventManager.Queue<KeyPressedEvent>(
        event
    );
}

void
Game::OnKeyReleased( sf::Event::KeyEvent someEvent )
{
}

void
Game::Update( sf::Time someDeltaTime )
{
    if( !_isInitialized )
    {
        _isInitialized = true;
        this->OnInitialze();

        return;
    }

    _systems.Get<ActorMoveSystem>()->Update( someDeltaTime );

    _eventManager.Update( someDeltaTime );

    _processes.Update( someDeltaTime );

    auto list = 
        _entityManager.GetEntitiesWithTag( "PLAYER" );
        
    std::size_t playerId = list.front();    

    auto pos =
        _entityManager.GetEntityDataOfType<PositionData>( playerId );
    auto camera =
        _entityManager.GetEntityDataOfType<CameraData>( playerId );

    camera->view.setCenter(
        pos->x * TileSize + (TileSize / 2.0f),
        pos->y * TileSize + (TileSize / 2.0f)
    );
}

void
Game::Render( sf::Time someDeltaTime, sf::RenderTarget& someRenderTarget )
{
    if( ! _isInitialized )
    {
        return;
    }

    _systems.Get<RenderMapSystem>()->Update( someDeltaTime );

    _systems.Get<SpriteSheetAnimationSystem>()->Update( someDeltaTime );

    _systems.Get<RenderFpsSystem>()->Update( someDeltaTime );
}
/*SfmlGame*/

/*IEventHandler*/
bool
Game::Handle( std::shared_ptr<BlurryRoots::Yanecos::IEvent> someEvent )
{
    return false;
}
/*IEventHandler*/
