#include "zpch.hpp"

#include <fstream>

#include "Player.hpp"
#include "Zombie.hpp"
#include "Bullet.hpp"
#include "Pickup.hpp"
#include "HUD.hpp"

const std::string g_gameData{ "scores.txt" };

using sf::Keyboard;

int main()
{
	srand(static_cast<int>(time(0)));

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
	int wave{};

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

	// read high score from file
	std::ifstream inputFile(g_gameData);
	if (inputFile.is_open())
	{
		inputFile >> highScore;
		inputFile.close();
	}

	HUD playerHUD(player, score, highScore, numZombiesAlive);
	sf::View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	
	// how often to update the HUD
	float hudTimeInterval = 0.1f;
	sf::Time hudTimeSinceLastUpdate;

	sf::Font font;
	font.loadFromFile("../assets/fonts/zombiecontrol.ttf");

	// game paused
	sf::Text pausedText;
	pausedText.setFont(font);
	pausedText.setFillColor(sf::Color::White);
	pausedText.setCharacterSize(155);
	pausedText.setPosition(560, 400);
	pausedText.setString("Press Enter\nto continue");

	// wave number
	sf::Text waveText;
	waveText.setFont(font);
	waveText.setCharacterSize(55);
	waveText.setFillColor(sf::Color::White);
	waveText.setPosition(1200, 1100);

	// level up
	sf::Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(sf::Color::White);
	levelUpText.setPosition(150, 350);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1 - Increased rate of fire\n" <<
		"2 - Increased clip size\n" <<
		"3 - Increased max health\n" <<
		"4 - Increased run speed\n" <<
		"5 - More and better health pickups\n" <<
		"6 - More and better ammo pickups\n";
	levelUpText.setString(levelUpStream.str());

	// game over
	sf::Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setPosition(400, 950);
	gameOverText.setString("Press Enter to play");

	sf::Text gameText;
	gameText.setFont(font);
	gameText.setCharacterSize(125);
	gameText.setFillColor(sf::Color(180, 0, 0));
	gameText.setPosition(400, 100);
	gameText.setString("Zombie Appocalypse");

	sf::Sprite gameOverSprite;
	gameOverSprite.setTexture(TextureHolder::getTexture("../assets/gfx/background.png"));
	gameOverSprite.setPosition(0, 0);

	sf::SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("../assets/sfx/powerup.wav");
	sf::Sound powerupSound;
	powerupSound.setVolume(GamePlay::sfxVolume);
	powerupSound.setBuffer(powerupBuffer);

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
			// Let the player choose an upgrade for each wave
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
			{
				// increased rate of fire
				player.upgradeFireRate();
				state = State::PLAYING;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
			{
				// increased clip size
				player.upgradeClipSize();
				state = State::PLAYING;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
			{
				// increased max health
				player.upgradeHealth();
				state = State::PLAYING;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
			{
				// increased run speed
				player.upgradeSpeed();
				state = State::PLAYING;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
			{
				// more and better health pickups
				healthPickup.upgrade();
				state = State::PLAYING;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
			{
				// more and better ammo pickups
				ammoPickup.upgrade();
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				arena.width = 1000 + 300 * wave;
				arena.height = 1000 + 300 * wave;
				arena.left = 0;
				arena.top = 0;

				int tileSize = GamePlay::createBackground(background, arena);

				// create the player
				player.spawn(arena, resolution, tileSize);

				// update pickups with new arena
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				// create the zombie horde
				numZombies = 10 + 3 * wave;
				numZombiesAlive = numZombies;
				zombieHorde = GamePlay::createHorde(numZombies, arena);

				powerupSound.play();

				++wave;

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

			// update even though not spawned, since we calculate the time until next spawn
			healthPickup.update(dt.asSeconds());
			ammoPickup.update(dt.asSeconds());

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

						// save high score to file
						std::ofstream outputFile(g_gameData);
						outputFile << highScore;
						outputFile.close();

						// reset all stats
						wave = 0;
						score = 0;
						player.resetPlayerStats();
						ammoPickup.resetStats();
						healthPickup.resetStats();
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

		window.clear();

		if (state == State::PLAYING)
		{
			window.setView(mainView);

			// draw background
			window.draw(background, &backgroundTexture);

			// render dead zombies first, i.e., the blood splat
			for (auto& zombie : zombieHorde)
			{
				if (!zombie->isAlive()) window.draw(zombie->getSprite());
			}

			if (healthPickup.isSpawned()) window.draw(healthPickup.getSprite());
			if (ammoPickup.isSpawned()) window.draw(ammoPickup.getSprite());

			window.draw(player.getSprite());

			for (auto& zombie : zombieHorde)
			{
				if (zombie->isAlive()) window.draw(zombie->getSprite());
			}

			for (auto& bullet : player.getBullets())
			{
				if (!bullet.isInFlight()) continue;

				window.draw(bullet.getShape());
			}

			window.draw(crosshairSprite);

			// render HUD
			window.setView(hudView);
			playerHUD.render(window);
			std::stringstream waveStream;
			waveStream << "Wave: " << wave;
			waveText.setString(waveStream.str());
			window.draw(waveText);

		}
		else if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}
		else if (state == State::LEVELING_UP)
		{
			window.draw(gameOverSprite);
			window.draw(levelUpText);
		}
		else if (state == State::GAME_OVER)
		{
			window.draw(gameOverSprite);
			window.draw(gameOverText);
			window.draw(gameText);
		}

		window.display();
	}
}