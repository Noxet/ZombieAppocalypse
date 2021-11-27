#include "zpch.hpp"

#include "Pickup.hpp"


Pickup::Pickup(PickupType type)
{
	m_type = type;

	switch (m_type)
	{
	case PickupType::Health:
		// health
		m_sprite = sf::Sprite(TextureHolder::getTexture("../assets/gfx/health_pickup.png"));
		m_value = HEALTH_START_VALUE;
		break;

	case PickupType::Ammo:
		m_sprite = sf::Sprite(TextureHolder::getTexture("../assets/gfx/ammo_pickup.png"));
		m_value = AMMO_START_VALUE;
		break;
	}

	m_sprite.setOrigin(m_sprite.getTexture()->getSize().x / 2.f, m_sprite.getTexture()->getSize().y / 2.f);

	m_secondsToLive = START_SECONDS_TO_LIVE;
	m_secondsToWait = START_WAIT_TIME;

	pickupBuffer.loadFromFile("../assets/sfx/pickup.wav");
	pickupSound.setBuffer(pickupBuffer);
	pickupSound.setVolume(GamePlay::sfxVolume);
}


void Pickup::update(float dt)
{
	if (m_spawned)
	{
		m_secondsSinceSpawn += dt;
	}
	else
	{
		m_secondsSinceDeSpawn += dt;
	}

	if (m_spawned && m_secondsSinceSpawn > m_secondsToLive)
	{
		// hide it
		m_spawned = false;
		m_secondsSinceDeSpawn = 0;
	}

	if (!m_spawned && m_secondsSinceDeSpawn > m_secondsToWait)
	{
		// spawn a new pickup
		spawn();
	}

}


void Pickup::setArena(sf::IntRect arena)
{
	m_arena = arena;
	spawn();
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
	pickupSound.play();
	return m_value;
}


void Pickup::upgrade()
{
	switch (m_type)
	{
	case PickupType::Health:
		m_value += (HEALTH_START_VALUE * 0.5f);
		break;

	case PickupType::Ammo:
		m_value += (AMMO_START_VALUE * 0.5f);
		break;
	}

	// make the pickups more frequent and longer lasting
	m_secondsToLive += (START_SECONDS_TO_LIVE / 10.f);
	m_secondsToWait += (START_WAIT_TIME / 10.f);
}

