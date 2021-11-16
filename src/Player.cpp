#include "zpch.hpp"

#include "Player.hpp"


Player::Player()
{
	m_speed = START_SPEED;
	m_health = START_HEALTH;
	m_maxHealth = START_HEALTH;

	m_texture.loadFromFile("../assets/gfx/player.png");
	m_sprite.setTexture(m_texture);
	// set the origin to the sprite center (allows for easy rotation)
	m_sprite.setOrigin(25, 25);
}


void Player::spawn(sf::IntRect arena, sf::Vector2f resolution, int tileSize)
{
	// spawn in the center
	m_position.x = static_cast<float>(arena.width) / 2.f;
	m_position.y = static_cast<float>(arena.height) / 2.f;

	// copy the arena layout
	m_arena.left = arena.left;
	m_arena.width = arena.width;
	m_arena.top = arena.top;
	m_arena.height = arena.height;

	m_tileSize = tileSize;

	m_resolution.x = resolution.x;
	m_resolution.y = resolution.y;
}


void Player::resetPlayerStats()
{
	m_speed = START_SPEED;
	m_health = START_HEALTH;
	m_maxHealth = START_HEALTH;
}


bool Player::hit(sf::Time timeHit)
{
}


sf::Time Player::getLastHit()
{
}


sf::FloatRect Player::getPosition()
{
}


sf::Vector2f Player::getCenter()
{
}


float Player::getRotation()
{
}


void Player::moveLeft()
{
}


void Player::stopLeft()
{
}


void Player::moveRight()
{
}


void Player::stopRight()
{
}


void Player::moveUp()
{
}


void Player::stopUp()
{
}


void Player::moveDown()
{
}


void Player::stopDown()
{
}


void Player::update(float dt, sf::Vector2i mousePosition)
{
}


void Player::upgradeSpeed()
{
}


void Player::upgradeHealth()
{
}


void Player::increaseHealthLevel(int amount)
{
}


int Player::getHealth()
{
}
