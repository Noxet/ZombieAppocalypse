#include "zpch.hpp"

#include "Player.hpp"


Player::Player() : m_bullets(100)
{
	m_speed = START_SPEED;
	m_health = START_HEALTH;
	m_maxHealth = START_HEALTH;

	m_sprite = sf::Sprite(TextureHolder::getTexture("../assets/gfx/player.png"));
	// set the origin to the sprite center (allows for easy rotation)
	m_sprite.setOrigin(25, 25);
	m_tileSize = 0;
	m_timeSinceLastFired = 0;

	// load all sounds
	hitBuffer.loadFromFile("../assets/sfx/hit.wav");
	hitSound.setBuffer(hitBuffer);
	hitSound.setVolume(GamePlay::sfxVolume);

	shootBuffer.loadFromFile("../assets/sfx/shoot.wav");
	shootSound.setBuffer(shootBuffer);
	shootSound.setVolume(GamePlay::sfxVolume);

	reloadBuffer.loadFromFile("../assets/sfx/reload.wav");
	reloadSound.setBuffer(reloadBuffer);
	reloadSound.setVolume(GamePlay::sfxVolume);

	reloadFailedBuffer.loadFromFile("../assets/sfx/reload_failed.wav");
	reloadFailedSound.setBuffer(reloadFailedBuffer);
	reloadFailedSound.setVolume(GamePlay::sfxVolume);
}


void Player::update(float dt, sf::Vector2i mousePosition)
{
	// handle movement, allow to run diagonally
	if (m_upPressed)
	{
		m_position.y -= m_speed * dt;
		const auto upperBound = static_cast<float>(m_arena.top + m_tileSize);
		if (m_position.y < upperBound) m_position.y = upperBound;
	}

	if (m_downPressed)
	{
		m_position.y += m_speed * dt;
		const auto upperBound = static_cast<float>(m_arena.top + m_arena.height - m_tileSize);
		if (m_position.y > upperBound) m_position.y = upperBound;
	}

	if (m_leftPressed)
	{
		m_position.x -= m_speed * dt;
		const auto upperBound = static_cast<float>(m_arena.left + m_tileSize);
		if (m_position.x < upperBound) m_position.x = upperBound;
	}

	if (m_rightPressed)
	{
		m_position.x += m_speed * dt;
		const auto upperBound = static_cast<float>(m_arena.left + m_arena.width - m_tileSize);
		if (m_position.x > upperBound) m_position.x = upperBound;
	}

	/*
	 * Use resolution instead of player position, so we can manage a different game resolution compared to the screen res.
	 * If the game res is 800x600, the center is <400, 300>, whereas the screen center (for 1920x1080 res) is <960, 540>
	 */
	const float rot = std::atan2f(mousePosition.y - m_resolution.y / 2.f, mousePosition.x - m_resolution.x / 2.f) * 180 / za::PI;
	m_sprite.setRotation(rot);

	m_sprite.setPosition(m_position);

	m_timeSinceLastFired += dt;
}


void Player::spawn(sf::IntRect arena, sf::Vector2u resolution, int tileSize)
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

	m_sprite.setPosition(m_position);
}


void Player::resetPlayerStats()
{
	m_speed = START_SPEED;
	m_health = START_HEALTH;
	m_maxHealth = START_HEALTH;
	m_bulletsSpare = START_BULLETS_SPARE;
	m_clipSize = START_CLIP_SIZE;
	m_bulletsInClip = m_clipSize;
	m_fireRate = START_FIRE_RATE;
}


void Player::shoot(Vector2f target)
{
	if (m_bulletsInClip > 0 && m_timeSinceLastFired > 1.f / m_fireRate)
	{
		m_bullets[m_currentBullet].shoot(getCenter(), target);
		++m_currentBullet;
		// only have a fixed amount of bullets spawned at the same time, then we reuse
		if (m_currentBullet >= m_bullets.size()) m_currentBullet = 0;

		--m_bulletsInClip;
		m_timeSinceLastFired = 0;

		shootSound.play();
	}

	if (m_bulletsInClip <= 0)
	{
		// play empty clip sound?
	}
}


void Player::reload()
{
	const int bulletsToFill = m_clipSize - m_bulletsInClip;
	if (bulletsToFill <= m_bulletsSpare)
	{
		m_bulletsSpare -= bulletsToFill;
		m_bulletsInClip += bulletsToFill;

		reloadSound.play();
	}
	else if (m_bulletsSpare > 0)
	{
		// not enough to fill the clip, use the remaining
		m_bulletsInClip += m_bulletsSpare;
		m_bulletsSpare = 0;

		reloadSound.play();
	}
	else
	{
		reloadFailedSound.play();
	}
}


void Player::addAmmo(int ammo)
{
	m_bulletsSpare += ammo;
}


bool Player::hit(sf::Time timeHit)
{
	// prevent to get hit too often
	if (timeHit.asMilliseconds() - m_lastHit.asMilliseconds() > 200)
	{
		m_lastHit = timeHit;
		m_health -= 10;
		hitSound.play();
		return true;
	}

	return false;
}


sf::Time Player::getLastHit() const
{
	return m_lastHit;
}


sf::FloatRect Player::getPosition() const
{
	return m_sprite.getGlobalBounds();
}


sf::Vector2f Player::getCenter() const
{
	return m_position;
}


float Player::getRotation() const
{
	return m_sprite.getRotation();
}


sf::Sprite Player::getSprite() const
{
	return m_sprite;
}


void Player::moveLeft()
{
	m_leftPressed = true;
}


void Player::stopLeft()
{
	m_leftPressed = false;
}


void Player::moveRight()
{
	m_rightPressed = true;
}


void Player::stopRight()
{
	m_rightPressed = false;
}


void Player::moveUp()
{
	m_upPressed = true;
}


void Player::stopUp()
{
	m_upPressed = false;
}


void Player::moveDown()
{
	m_downPressed = true;
}


void Player::stopDown()
{
	m_downPressed = false;
}


void Player::upgradeSpeed()
{
	m_speed += START_SPEED * 0.2f;
}


void Player::upgradeHealth()
{
	m_maxHealth += static_cast<int>(START_HEALTH * 0.2);
}


void Player::increaseHealthLevel(int amount)
{
	m_health += amount;
	m_health = m_health > m_maxHealth ? m_maxHealth : m_health;
}


void Player::upgradeFireRate()
{
	++m_fireRate;
}


void Player::upgradeClipSize()
{
	m_clipSize += 3;
}


int Player::getHealth() const
{
	return m_health;
}
