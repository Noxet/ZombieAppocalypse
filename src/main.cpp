#include "zpch.hpp"

#include "Player.hpp"
#include "SFML/Graphics.hpp"

using sf::Keyboard;

int main()
{
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

	State state = State::GAME_OVER;

	// get resolution from screen
	sf::Vector2u resolution(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);

	sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Appocalypse", sf::Style::Fullscreen);
	//window.setFramerateLimit(60);

	// the "camera" view, initially placed at the screen origin
	sf::View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	sf::Clock clock;
	sf::Time gameTimeTotal;

	sf::Vector2f mouseWorldPosition;	// relative to game world coordinates
	sf::Vector2i mouseScreenPosition;	// relative to screen coordinates (in px)

	Player player;

	// the boundaries of the arena
	sf::IntRect arena;

	while (window.isOpen())
	{
		/*
		 * Handle input
		 */

		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					// reset clock to prevent frame jump
					clock.restart();
					state = State::PLAYING;
				}
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					// start from level 1 again
					state = State::LEVELING_UP;
				}
			}
		}

		// handle quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// handle player movement
		if (state == State::PLAYING)
		{
			Keyboard::isKeyPressed(Keyboard::W) ? player.moveUp() : player.stopUp();
			Keyboard::isKeyPressed(Keyboard::S) ? player.moveDown() : player.stopDown();
			Keyboard::isKeyPressed(Keyboard::A) ? player.moveLeft() : player.stopLeft();
			Keyboard::isKeyPressed(Keyboard::D) ? player.moveRight() : player.stopRight();
		}
		else if (state == State::LEVELING_UP)
		{
			state = State::PLAYING;
		}

		sf::Vector2i mPos = sf::Mouse::getPosition();
		std::cout << std::format("Mouse: ({}, {})", mPos.x, mPos.y) << std::endl;

		player.update(clock.getElapsedTime().asSeconds(), mPos);
		clock.restart();

		window.clear();
		window.draw(player.getSprite());
		window.display();
	}
}