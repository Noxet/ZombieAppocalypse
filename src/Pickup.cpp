#include "zpch.hpp"

#include "Pickup.hpp"


Pickup::Pickup(int type, sf::IntRect arena)
{
	m_type = type;

	m_arena = arena;

	switch (m_type)
	{
	case 1:
		// health
		m_sprite = sf::Sprite(TextureHolder::getTexture("../assets/gfx/health_pickup.png"));
		m_value = HEALTH_START_VALUE;
		break;

	case 2:
		m_sprite = sf::Sprite(TextureHolder::getTexture("../assets/gfx/ammo_pickup.png"));
		m_value = AMMO_START_VALUE;
		break;

	default:
		// TODO: error
		break;
	}

	m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2.f, m_sprite.getTexture()->getSize().y / 2.f);

	m_secondsToLive = START_SECONDS_TO_LIVE;
	m_secondsToWait = START_WAIT_TIME;
}


void Pickup::update(float dt)
{
}


void Pickup::spawn()
{
	const int x = rand() % m_arena.width;
	const int y = rand() % m_arena.height;

	m_secondsSinceSpawn = 0;
	m_spawned = true;

	m_sprite.setPosition(x, y);
}


int Pickup::getPickup()
{
	m_spawned = false;
	m_secondsSinceDeSpawn = 0;
	return m_value;
}


void Pickup::upgrade()
{
}

