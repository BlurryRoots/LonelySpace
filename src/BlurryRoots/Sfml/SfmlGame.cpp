#include "SfmlGame.hpp"

#include <sstream>

namespace BlurryRoots
{
	namespace Sfml
	{
		std::shared_ptr<sf::RenderWindow> SfmlGame::getRenderWindow()
		{
			return this->renderWindow;
		}

		SfmlGame::SfmlGame( std::shared_ptr<sf::RenderWindow> someRenderWindow )
			:
				renderWindow( someRenderWindow ),
				lastEventCount( 0 ),
				isRunning( false )
		{
		}

		SfmlGame::~SfmlGame()
		{
			if( this->renderWindow->isOpen() )
			{
				this->renderWindow->close();
			}
		}

		void SfmlGame::OnClosed()
		{
		}

		void SfmlGame::OnResize( sf::Event::SizeEvent someEvent )
		{
		}

		void SfmlGame::OnLostFocus()
		{
		}

		void SfmlGame::OnGainedFocus()
		{
		}

		void SfmlGame::OnTextEntered( sf::Event::TextEvent someEvent )
		{
		}

		void SfmlGame::OnKeyPressed( sf::Event::KeyEvent someEvent )
		{
		}

		void SfmlGame::OnKeyReleased( sf::Event::KeyEvent someEvent )
		{
		}

		void SfmlGame::OnMouseWheelMoved( sf::Event::MouseWheelEvent someEvent )
		{
		}

		void SfmlGame::OnMouseButtonPressed( sf::Event::MouseButtonEvent someEvent )
		{
		}

		void SfmlGame::OnMouseButtonReleased(
				sf::Event::MouseButtonEvent someEvent )
		{
		}

		void SfmlGame::OnMouseMoved( sf::Event::MouseMoveEvent someEvent )
		{
		}

		void SfmlGame::OnMouseEntered()
		{
		}

		void SfmlGame::OnMouseLeft()
		{
		}

		void SfmlGame::OnJoystickButtonPressed(
				sf::Event::JoystickButtonEvent someEvent )
		{
		}

		void SfmlGame::OnJoystickButtonReleased(
				sf::Event::JoystickButtonEvent someEvent )
		{
		}

		void SfmlGame::OnJoystickMoved( sf::Event::JoystickMoveEvent someEvent )
		{
		}

		void SfmlGame::OnJoystickConnected(
				sf::Event::JoystickConnectEvent someEvent )
		{
		}
		
		void SfmlGame::OnJoystickDisconnected(
				sf::Event::JoystickConnectEvent someEvent )
		{
		}

		void SfmlGame::Quit()
		{
			this->isRunning = false;
		}

		void SfmlGame::Run()
		{
			this->isRunning = this->renderWindow != nullptr
			               && this->renderWindow->isOpen();

			sf::Clock diffTimeClock;
			sf::Time deltaTime;
			while( isRunning )
			{
				deltaTime = diffTimeClock.restart();

				//update
				this->Update( deltaTime );

				sf::Event event;
				int count = 0;
				while( this->renderWindow->pollEvent( event ) )
				{
					if( event.type == sf::Event::Closed )
					{
						this->OnClosed();
						++count;
					}

					if( event.type == sf::Event::Resized )
					{
						this->OnResize( event.size );
						++count;
					}

					if( event.type == sf::Event::LostFocus )
					{
						this->OnLostFocus();
						++count;
					}

					if( event.type == sf::Event::GainedFocus )
					{
						this->OnGainedFocus();
						++count;
					}

					if( event.type == sf::Event::TextEntered )
					{
						this->OnTextEntered( event.text );
						++count;
					}

					if( event.type == sf::Event::KeyPressed )
					{
						this->OnKeyPressed( event.key );
						++count;
					}

					if( event.type == sf::Event::KeyReleased )
					{
						this->OnKeyReleased( event.key );
						++count;
					}

					if( event.type == sf::Event::MouseWheelMoved )
					{
						this->OnMouseWheelMoved( event.mouseWheel );
						++count;
					}

					if( event.type == sf::Event::MouseButtonPressed )
					{
						this->OnMouseButtonPressed( event.mouseButton );
						++count;
					}

					if( event.type == sf::Event::MouseButtonReleased )
					{
						this->OnMouseButtonReleased( event.mouseButton );
						++count;
					}

					if( event.type == sf::Event::MouseMoved )
					{
						this->OnMouseMoved( event.mouseMove );
						++count;
					}

					if( event.type == sf::Event::MouseEntered )
					{
						this->OnMouseEntered();
						++count;
					}

					if( event.type == sf::Event::MouseLeft )
					{
						this->OnMouseLeft();
						++count;
					}

					if( event.type == sf::Event::JoystickButtonPressed )
					{
						this->OnJoystickButtonPressed( event.joystickButton );
						++count;
					}

					if( event.type == sf::Event::JoystickButtonReleased )
					{
						this->OnJoystickButtonReleased( event.joystickButton );
						++count;
					}

					if( event.type == sf::Event::JoystickMoved )
					{
						this->OnJoystickMoved( event.joystickMove );
						++count;
					}

					if( event.type == sf::Event::JoystickConnected )
					{
						this->OnJoystickConnected( event.joystickConnect );
						++count;
					}

					if( event.type == sf::Event::JoystickDisconnected )
					{
						this->OnJoystickDisconnected( event.joystickConnect );
						++count;
					}

					if( event.type == sf::Event::Count )
					{
						this->lastEventCount = event.Count;

						if( this->lastEventCount != count )
						{
							std::stringstream ss;

							ss 	<< "Event count difference! Counted: "
								<< this->lastEventCount 
								<< " Expected: " << event.Count;

							this->OnError( ss.str() );
						}
					}
				}

				//render
				this->renderWindow->clear();
				this->Render( deltaTime, *( this->renderWindow ) );
				this->renderWindow->display();
			}
		}
	}
}
