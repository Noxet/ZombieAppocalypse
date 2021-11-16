#pragma once

#include "SFML/Graphics.hpp"

class Player
{
private:
	const float START_SPEED{ 200 };
	const int START_HEALTH{ 100 };

	// player data
	sf::Vector2f m_position;
	bool m_upPressed;
	bool m_downPressed;
	bool m_leftPressed;
	bool m_rightPressed;
	int m_health;
	int m_maxHealth;
	sf::Time m_lastHit;		// when the player was last hit
	float m_speed;		// player speed in px/s

	// graphics
	sf::Sprite m_sprite;
	sf::Texture m_texture;

	// game info
	sf::Vector2f m_resolution;	// current screen res
	sf::IntRect m_arena;		// size of current arena
	int m_tileSize;			// tile size of the arena

public:
	Player();

	void update(float dt, sf::Vector2i mousePosition);

	// set player position, angle etc.
	void spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize);

	// call at the end of every game
	void resetPlayerStats();

	// handle getting hit by a zombie
	bool hit(sf::Time timeHit);
	sf::Time getLastHit();

	// get player position as a rectangle, useful for collision detection
	sf::FloatRect getPosition();
	// get player center in <x, y>
	sf::Vector2f getCenter();
	// get player facing angle, in degrees
	float getRotation();

	sf::Sprite getSprite();

	// move/stop moving the player
	void moveLeft();
	void stopLeft();

	void moveRight();
	void stopRight();

	void moveUp();
	void stopUp();

	void moveDown();
	void stopDown();

	// player boosts
	void upgradeSpeed();
	void upgradeHealth();
	void increaseHealthLevel(int amount);	// increase the max health

	int getHealth();
};
