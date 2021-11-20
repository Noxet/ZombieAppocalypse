#include "zpch.hpp"

#include "Bullet.hpp"


Bullet::Bullet()
{
	m_inFlight = false;

	m_bulletShape = sf::RectangleShape(Vector2f(4, 2));
	m_bulletShape.setOrigin(2, 1);
	m_bulletShape.setFillColor(sf::Color::Yellow);
}


void Bullet::update(float dt)
{
	if (!m_inFlight) return;

	const float dx = m_bulletSpeed * std::cos(m_bulletShape.getRotation() * za::PI / 180) * dt;
	const float dy = m_bulletSpeed * std::sin(m_bulletShape.getRotation() * za::PI / 180) * dt;

	m_position.x += dx;
	m_position.y += dy;

	m_bulletShape.setPosition(m_position);
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
