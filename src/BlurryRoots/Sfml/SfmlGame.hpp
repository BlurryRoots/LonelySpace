
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <memory>

#ifndef _SFMLGAME_
#define _SFMLGAME_

namespace BlurryRoots
{
	namespace Sfml
	{
		class SfmlGame
		{
			private:

				std::shared_ptr<sf::RenderWindow> renderWindow;

				int lastEventCount;

				bool isRunning;

			protected:

				std::shared_ptr<sf::RenderWindow> getRenderWindow();

			public:

				SfmlGame( std::shared_ptr<sf::RenderWindow> someRenderWindow );

				virtual
					~SfmlGame();

				virtual
					void Update( sf::Time deltaTime ) = 0;

				virtual
					void Render(
						sf::Time deltaTime,
						sf::RenderTarget& someRenderTarget ) = 0;

				virtual 
					void OnClosed();

				virtual 
					void OnResize(
						sf::Event::SizeEvent someEvent );

				virtual 
					void OnLostFocus();

				virtual 
					void OnGainedFocus();

				virtual 
					void OnTextEntered(
						sf::Event::TextEvent someEvent );

				virtual 
					void OnKeyPressed(
						sf::Event::KeyEvent someEvent );

				virtual
					void OnKeyReleased(
						sf::Event::KeyEvent someEvent );

				virtual 
					void OnMouseWheelMoved(
						sf::Event::MouseWheelEvent someEvent );

				virtual 
					void OnMouseButtonPressed(
						sf::Event::MouseButtonEvent someEvent );

				virtual 
					void OnMouseButtonReleased(
						sf::Event::MouseButtonEvent someEvent );

				virtual 
					void OnMouseMoved(
						sf::Event::MouseMoveEvent someEvent );

				virtual 
					void OnMouseEntered();

				virtual 
					void OnMouseLeft();

				virtual 
					void OnJoystickButtonPressed(
						sf::Event::JoystickButtonEvent someEvent );

				virtual 
					void OnJoystickButtonReleased(
						sf::Event::JoystickButtonEvent someEvent );

				virtual 
					void OnJoystickMoved(
						sf::Event::JoystickMoveEvent someEvent );

				virtual 
					void OnJoystickConnected(
						sf::Event::JoystickConnectEvent someEvent );

				virtual 
					void OnJoystickDisconnected(
						sf::Event::JoystickConnectEvent someEvent );


				virtual 
					void OnError(
						std::string someError ) = 0;

				void Quit();


				void Run();
		};
	}
}

#endif
