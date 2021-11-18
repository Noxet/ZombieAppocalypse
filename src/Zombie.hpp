#pragma once

class Zombie
{
private:
	// for different types of zombies
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;

	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;

	// vary the speed of zombies
	const int MAX_VARIANCE = 30;
	const int OFFSET = 101 - MAX_VARIANCE;

	Vector2f m_position;
	sf::Sprite m_sprite;

	float m_speed{};

	float m_health{};

	bool m_bAlive{};

public:
	void update(float dt, Vector2f playerLocation);

	/**
	 * Take damage.
	 * @return True if the hit killed the zombie, else false
	 */
	bool hit();

	bool isAlive() const;

	void spawn(float startX, float startY, int type);

	sf::FloatRect getPosition() const;

	sf::Sprite getSprite() const;
};
