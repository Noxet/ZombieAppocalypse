#include "zpch.hpp"

#include "Zombie.hpp"
#include "TextureHolder.hpp"


void Zombie::update(float dt, Vector2f playerLocation)
{
	// TODO: instead of normalizing based on max, we should normalize on the resultant vector
	const Vector2f delta = playerLocation - m_position;
	const float max = std::max(std::abs(delta.x), std::abs(delta.y));

	// we don't need to compute stuff if we are close
	if (max > 10.f)
	{
		const Vector2f deltaNorm = delta / max;
		m_position.x += deltaNorm.x * m_speed * dt;
		m_position.y += deltaNorm.y * m_speed * dt;
		m_sprite.setPosition(m_position);
		m_sprite.setRotation(std::atan2(delta.y, delta.x) * 180.f / za::PI);
	}
}


bool Zombie::hit()
{
	--m_health;
	if (m_health < 0)
	{
		m_bAlive = false;
		// replace the zombie image with blood splat
		m_sprite.setTexture(TextureHolder::getTexture("../assets/gfx/blood.png"));
		return true;
	}

	return false;
}


bool Zombie::isAlive() const
{
	return m_bAlive;
}


void Zombie::spawn(float startX, float startY, int type)
{
	switch (type)
	{
	case 0:
		// Bloater
		m_sprite = sf::Sprite(TextureHolder::getTexture("../assets/gfx/bloater.png"));
		m_health = BLOATER_HEALTH;
		m_speed = BLOATER_SPEED;
		break;

	case 1:
		// Chaser
		m_sprite = sf::Sprite(TextureHolder::getTexture("../assets/gfx/chaser.png"));
		m_health = CHASER_HEALTH;
		m_speed = CHASER_SPEED;
		break;

	case 2:
		// Crawler
		m_sprite = sf::Sprite(TextureHolder::getTexture("../assets/gfx/crawler.png"));
		m_health = CRAWLER_HEALTH;
		m_speed = CRAWLER_SPEED;
		break;

	default:
		// TODO: warning
		break;
	}

	// modify each zombie to make them unique.
	// modifier is between 71 and 100
	const float modifier = static_cast<float>((rand() % MAX_VARIANCE) + OFFSET) / 100.f;
	m_speed *= modifier;

	m_position.x = startX;
	m_position.y = startY;

	// center origin
	m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2.f, m_sprite.getTexture()->getSize().y / 2.f);
	m_sprite.setPosition(m_position);
}


sf::FloatRect Zombie::getPosition() const
{
	return m_sprite.getGlobalBounds();
}


sf::Sprite Zombie::getSprite() const
{
	return m_sprite;
}
