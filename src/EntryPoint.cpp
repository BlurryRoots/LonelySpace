

#include "Game.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <memory>

int main()
{
	Game game(
		std::make_shared<sf::RenderWindow>(
			sf::VideoMode( 800, 600 ),
			"LudumDare28"
		)
	);

	game.Run();

    /*std::make_shared<Game>(
        std::make_shared<sf::RenderWindow>(
            sf::VideoMode( 800, 600 ),
            "LudumDare28"
        )
    )->Run();*/

	return 0;
}
