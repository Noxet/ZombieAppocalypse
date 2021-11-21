#pragma once

class Pickup
{
private:
	constexpr int HEALTH_START_VALUE{ 50 };
	constexpr int AMMO_START_VALUE{ 12 };
	// time until next pickups spawns
	constexpr int START_WAIT_TIME{ 10 };
	// only show for x amount of seconds
	constexpr int START_SECONDS_TO_LIVE{ 5 };

	sf::Sprite m_sprite;

	// the arena the pickup exists in
	sf::IntRect m_arena;

	// the pickup's worth
	int m_value;

	// what type of pickup?
	// 1 = health, 2 = ammo
	int m_type{};

	bool m_spawned{};
	float m_secondsSinceSpawn{};
	float m_secondsSinceDeSpawn{};
	float m_secondsToLive{};
	float m_secondsToWait{};

public:
	Pickup(int type, sf::IntRect arena);

	void update(float dt);

	void spawn();

	sf::FloatRect getPosition() const { return m_sprite.getGlobalBounds(); }

	sf::Sprite getSprite() const { return m_sprite; }

	bool isSpawned() const { return m_spawned; }

	/**
	 * called when collision with player detected.
	 *
	 * @return the value of the pickup
	 */
	int getPickup();

	void upgrade();
};

