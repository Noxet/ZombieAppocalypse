#pragma once

class Bullet
{
private:
	Vector2f m_position;

	// we do not have a sprite for bullets, just make a rectangle instead
	sf::RectangleShape m_bulletShape;

	bool m_inFlight{ false };
	float m_bulletSpeed{ 1000 };

public:
	Bullet();

	void update(float dt);

	void stop();

	bool isInFlight() const;

	void shoot(Vector2f start, Vector2f target);

	// get bullet position in the world
	sf::FloatRect getPosition() const;

	// returns the actual shape, for drawing
	sf::RectangleShape getShape() const;
};
