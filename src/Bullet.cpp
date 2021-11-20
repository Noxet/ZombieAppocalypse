#include "zpch.hpp"

#include "Bullet.hpp"


Bullet::Bullet()
{
	m_inFlight = false;

	m_bulletShape = sf::RectangleShape(Vector2f(10, 5));
	m_bulletShape.setOrigin(5, 2.5f);
}


void Bullet::update(float dt)
{
}


void Bullet::stop()
{
	m_inFlight = false;
}


bool Bullet::isInFlight() const
{
	return m_inFlight;
}


void Bullet::shoot(const Vector2f start, const Vector2f target)
{
	m_position = start;
	m_inFlight = true;

	m_bulletShape.setPosition(start);

	const float angle = atan2(target.y - start.y, target.x - start.x) * 180 / za::PI;
	m_bulletShape.setRotation(angle);
}


sf::FloatRect Bullet::getPosition() const
{
	return m_bulletShape.getGlobalBounds();
}


sf::RectangleShape Bullet::getShape() const
{
	return m_bulletShape;
}
