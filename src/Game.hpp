
#include "Processes/LoadAssetProcess.hpp"

#include "MapCreator.hpp"

#include "Handlers/PlayerInputHandler.hpp"

#include <BlurryRoots/Sfml/SfmlGame.hpp>

#include <BlurryRoots/Yanecos/AssetManager.hpp>
#include <BlurryRoots/Yanecos/EntityManager.hpp>
#include <BlurryRoots/Yanecos/ProcessManager.hpp>
#include <BlurryRoots/Yanecos/SystemManager.hpp>
#include <BlurryRoots/Yanecos/EventManager.hpp>
#include <BlurryRoots/Yanecos/IEventHandler.hpp>
#include <BlurryRoots/Yanecos/IEvent.hpp>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

#ifndef __GAME__
#define __GAME__

class Game
		: public BlurryRoots::Sfml::SfmlGame,
		  public BlurryRoots::Yanecos::IEventHandler,
      	  public std::enable_shared_from_this<Game>
{
	private:

		bool _isInitialized;

		sf::Vector2f _currentMousePosition;

		BlurryRoots::Yanecos::AssetManager _assetManager;

		BlurryRoots::Yanecos::EntityManager _entityManager;

		BlurryRoots::Yanecos::ProcessManager _processes;

		BlurryRoots::Yanecos::SystemManager _systems;

		BlurryRoots::Yanecos::EventManager _eventManager;

		MapCreator _mapCreator;

		std::shared_ptr<PlayerInputHandler> _playerInputHandler;

		void
		OnInitialze();

		template<class T>
		void
		QueueAssetLoading( std::string somePath, std::string someKey )
		{
		    _processes.Add(
		        std::make_shared<LoadAssetProcess<T>>( 
		            _eventManager,
		            _assetManager,
		            somePath,
		            someKey
		        )
		    );
		}

		void
		SpawnEnemy( std::size_t someX, std::size_t someY );

	public:

		Game( std::shared_ptr<sf::RenderWindow> someRenderWindow )
		    : SfmlGame( someRenderWindow ),
		      _isInitialized( false ),
		      _currentMousePosition( 0, 0 ),
		      _mapCreator( _entityManager, _eventManager )
		{ }

		~Game()
		{
		    std::cout << "Game Destroyed!\n";
		}

		/*SfmlGame*/
		void
		OnError( std::string someError );

		void
		OnClosed();

		void
		OnMouseMoved( sf::Event::MouseMoveEvent someEvent );

		void
		OnMouseButtonPressed( sf::Event::MouseButtonEvent someEvent );

		void
		OnMouseButtonReleased( sf::Event::MouseButtonEvent someEvent );

		void
		OnKeyPressed( sf::Event::KeyEvent someEvent );

		void
		OnKeyReleased( sf::Event::KeyEvent someEvent );

		void
		Update( sf::Time deltaTime );

		void
		Render( sf::Time deltaTime, sf::RenderTarget& someRenderTarget );
		/*SfmlGame*/

		/*IEventHandler*/
		bool
        Handle( std::shared_ptr<BlurryRoots::Yanecos::IEvent> someEvent );
		/*IEventHandler*/
};

#endif
