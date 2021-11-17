#include "zpch.hpp"

#include "Player.hpp"
#include "SFML/Graphics.hpp"

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

int main()
{
	sf::VideoMode vm(WINDOW_WIDTH, WINDOW_HEIGHT);
	sf::RenderWindow window(vm, "Zombie Appocalypse", sf::Style::Fullscreen);
	window.setFramerateLimit(60);

	Player p;
	p.spawn(sf::IntRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT), 50);

	sf::Clock dt;

	while (window.isOpen())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ? p.moveUp() : p.stopUp();
		sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ? p.moveDown() : p.stopDown();
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ? p.moveLeft() : p.stopLeft();
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ? p.moveRight() : p.stopRight();

		sf::Vector2i mPos = sf::Mouse::getPosition();
		std::cout << std::format("Mouse: ({}, {})", mPos.x, mPos.y) << std::endl;

		p.update(dt.getElapsedTime().asSeconds(), mPos);
		dt.restart();

		window.clear();
		window.draw(p.getSprite());
		window.display();
	}
}