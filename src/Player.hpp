#pragma once

#include "Bullet.hpp"

class Player
{
private:
	static constexpr float START_SPEED{ 200 };
	static constexpr int START_HEALTH{ 100 };
	static constexpr int START_BULLETS_SPARE{ 24 };
	static constexpr int START_CLIP_SIZE{ 6 };
	static constexpr int START_FIRE_RATE{ 2 };

	// player data
	sf::Vector2f m_position{};
	bool m_upPressed{ false };
	bool m_downPressed{ false };
	bool m_leftPressed{ false };
	bool m_rightPressed{ false };
	int m_health{};
	int m_maxHealth{};
	sf::Time m_lastHit{};		// when the player was last hit
	float m_speed{};			// player speed in px/s

	// weapon
	std::vector<Bullet> m_bullets; // initialize bullets in ctor
	decltype(m_bullets.size()) m_currentBullet = 0;
	int m_bulletsSpare{ 24 };
	int m_bulletsInClip{ 6 };
	int m_clipSize{ 6 };
	float m_fireRate{ 2 };	// shots per second
	float m_timeSinceLastFired{};

	// graphics
	sf::Sprite m_sprite;
	sf::Texture m_texture;

	// audio
	sf::SoundBuffer hitBuffer;
	sf::Sound hitSound;
	sf::SoundBuffer shootBuffer;
	sf::Sound shootSound;
	sf::SoundBuffer reloadBuffer;
	sf::Sound reloadSound;
	sf::SoundBuffer reloadFailedBuffer;
	sf::Sound reloadFailedSound;

	// game info
	sf::Vector2u m_resolution;	// current screen res
	sf::IntRect m_arena;		// size of current arena
	int m_tileSize{};			// tile size of the arena

public:
	Player();

	void update(float dt, sf::Vector2i mousePosition);

	// set player position, angle etc.
	void spawn(sf::IntRect arena, sf::Vector2u resolution, int tileSize);

	// call at the end of every game
	void resetPlayerStats();

	void shoot(Vector2f target);
	void reload();
	void addAmmo(int ammo);

	int getBulletsSpare() const { return m_bulletsSpare; }
	int getBulletsInClip() const { return m_bulletsInClip; }
	int getClipSize() const { return m_clipSize; }

	// return a read-only reference to the bullets vector, for collision detection in the "game engine", aka main.
	std::vector<Bullet>& getBullets() { return m_bullets; }

	// handle getting hit by a zombie
	bool hit(sf::Time timeHit);
	sf::Time getLastHit() const;

	// get player position as a rectangle, useful for collision detection
	sf::FloatRect getPosition() const;
	// get player center in <x, y>
	sf::Vector2f getCenter() const;
	// get player facing angle, in degrees
	float getRotation() const;

	sf::Sprite getSprite() const;

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
	void upgradeFireRate();
	void upgradeClipSize();

	int getHealth() const;
};
