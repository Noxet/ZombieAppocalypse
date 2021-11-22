#include "zpch.hpp"

#include "SFML/Graphics.hpp"

#include "Player.hpp"
#include "GamePlay.hpp"
#include "Zombie.hpp"
#include "Bullet.hpp"
#include "Pickup.hpp"
#include "HUD.hpp"


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
	sf::Texture backgroundTexture(TextureHolder::getTexture("../assets/gfx/background_sheet.png"));

	// setup the crosshair
	window.setMouseCursorVisible(false);
	sf::Sprite crosshairSprite(TextureHolder::getTexture("../assets/gfx/crosshair.png"));
	crosshairSprite.setOrigin(crosshairSprite.getTexture()->getSize().x / 2.f, crosshairSprite.getTexture()->getSize().y / 2.f);

	// create some pickups
	Pickup healthPickup(PickupType::Health);
	Pickup ammoPickup(PickupType::Ammo);

	int score{};
	int highScore{};

	HUD playerHUD(player);
	sf::View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	
	// how often to update the HUD
	float hudTimeInterval = 0.1f;
	sf::Time hudTimeSinceLastUpdate;

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
				else if (event.key.code == Keyboard::R && state == State::PLAYING)
				{
					// reload weapon
					player.reload();
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

			// do not handle as an event since we want to have automatic firing
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				player.shoot(mouseWorldPosition);
			}
		}
		else if (state == State::LEVELING_UP)
		{
			state = State::PLAYING;

			if (state == State::PLAYING)
			{
				arena.width = 1000;
				arena.height = 1000;
				arena.left = 0;
				arena.top = 0;

				int tileSize = GamePlay::createBackground(background, arena);

				// create the player
				player.spawn(arena, resolution, tileSize);

				// update pickups with new arena
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

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
			mainView.setCenter(player.getCenter());

			crosshairSprite.setPosition(mouseWorldPosition);

			for (auto& zombie : zombieHorde)
			{
				if (!zombie->isAlive()) continue;

				zombie->update(dt.asSeconds(), player.getCenter());
			}

			for (auto& bullet : player.getBullets())
			{
				if (!bullet.isInFlight()) continue;

				bullet.update(dt.asSeconds());
			}

			if (healthPickup.isSpawned()) healthPickup.update(dt.asSeconds());
			if (ammoPickup.isSpawned()) ammoPickup.update(dt.asSeconds());

			// don't have to update the UI that often
			if (gameTimeTotal.asSeconds() - hudTimeSinceLastUpdate.asSeconds() > hudTimeInterval)
			{
				playerHUD.update(dt.asSeconds());
				hudTimeSinceLastUpdate = gameTimeTotal;
			}

			/*
			 * Collision detection
			 */
			for (const auto& zombie : zombieHorde)
			{
				// check if zombie has hit the player
				if (zombie->isAlive() && zombie->getPosition().intersects(player.getPosition()))
				{
					if (player.hit(gameTimeTotal))
					{
						std::cout << "Player hit!" << std::endl;
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
					}
				}

				// check if zombie has been hit by a bullet, or more
				for (auto& bullet : player.getBullets())
				{
					if (zombie->isAlive() && bullet.isInFlight() && zombie->getPosition().intersects(bullet.getPosition()))
					{
						bullet.stop();

						// check if the hit killed the zombie
						if (zombie->hit())
						{
							score += 10;
							--numZombiesAlive;

							// update high score
							if (score > highScore) highScore = score;
						}

						if (numZombiesAlive <= 0)
						{
							// wave completed
							state = State::LEVELING_UP;
						}
					}
				}
			}

			if (healthPickup.isSpawned() && healthPickup.getPosition().intersects(player.getPosition()))
			{
				int health = healthPickup.getPickup();
				player.increaseHealthLevel(health);
			}

			if (ammoPickup.isSpawned() && ammoPickup.getPosition().intersects(player.getPosition()))
			{
				player.addAmmo(ammoPickup.getPickup());
			}
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

			for (auto& bullet : player.getBullets())
			{
				if (!bullet.isInFlight()) continue;

				window.draw(bullet.getShape());
			}

			if (healthPickup.isSpawned()) window.draw(healthPickup.getSprite());
			if (ammoPickup.isSpawned()) window.draw(ammoPickup.getSprite());

			window.draw(crosshairSprite);

			// render HUD
			window.setView(hudView);
			playerHUD.render(window);

		}

		window.display();
	}
}