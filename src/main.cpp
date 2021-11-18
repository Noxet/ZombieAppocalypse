#include "zpch.hpp"

#include "SFML/Graphics.hpp"

#include "Player.hpp"
#include "GamePlay.hpp"
#include "TextureHolder.hpp"
#include "Zombie.hpp"


using sf::Keyboard;

int main()
{
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

	State state = State::GAME_OVER;

	// get resolution from screen
	sf::Vector2u resolution(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);

	sf::RenderWindow window(sf::VideoMode(resolution.x, resolution.y), "Zombie Appocalypse", sf::Style::Fullscreen);
	//window.setFramerateLimit(10);

	// the "camera" view, initially placed at the screen origin
	sf::View mainView(sf::FloatRect(0, 0, static_cast<float>(resolution.x), static_cast<float>(resolution.y)));

	sf::Clock clock;
	sf::Time gameTimeTotal;

	sf::Vector2f mouseWorldPosition;	// relative to game world coordinates
	sf::Vector2i mouseScreenPosition;	// relative to screen coordinates (in px)

	Player player;

	// zombie horde
	std::vector<std::unique_ptr<Zombie>> zombieHorde;
	int numZombies{};
	int numZombiesAlive{};

	// the boundaries of the arena
	sf::IntRect arena;

	// create background vertex array and loaf texture sheet
	sf::VertexArray background;
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("../assets/gfx/background_sheet.png");

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

			if (state == State::PLAYING)
			{
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				int tileSize = GamePlay::createBackground(background, arena);

				// create the player
				player.spawn(arena, resolution, tileSize);

				// create the zombie horde
				numZombies = 10;
				numZombiesAlive = 10;
				zombieHorde = GamePlay::createHorde(numZombies, arena);

				// prevent frame jump
				clock.restart();
			}
		}

		/*
		 * Update logic
		 */

		if (state == State::PLAYING)
		{
			sf::Time dt = clock.restart();
			gameTimeTotal += dt;

			// get mouse position (px) and convert to game world position
			mouseScreenPosition = sf::Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(mouseScreenPosition, mainView);

			player.update(dt.asSeconds(), mouseScreenPosition);

			for (auto& zombie : zombieHorde)
			{
				if (!zombie->isAlive()) continue;

				zombie->update(dt.asSeconds(), player.getCenter());
			}
			//z.update(dt.asSeconds(), player.getCenter());

			mainView.setCenter(player.getCenter());
		}

		/*
		 * Render
		 */
		if (state == State::PLAYING)
		{
			window.clear();
			window.setView(mainView);

			// draw background
			window.draw(background, &backgroundTexture);

			window.draw(player.getSprite());

			for (auto& zombie : zombieHorde)
			{
				window.draw(zombie->getSprite());
			}
			//window.draw(z.getSprite());
		}

		window.display();
	}
}