#include "zpch.hpp"

#include "HUD.hpp"
#include "Player.hpp"
#include <sstream>


/**
 * A static overlay to show stats
 */
HUD::HUD(Player& player) : m_player(player)
{
	m_gameOverSprite.setTexture(TextureHolder::getTexture("../assets/gfx/background.png"));
	m_gameOverSprite.setPosition(0, 0);

	m_ammoIcon.setTexture(TextureHolder::getTexture("../assets/gfx/ammo_icon.png"));
	m_ammoIcon.setPosition(20, 980);

	m_font.loadFromFile("../assets/fonts/zombiecontrol.ttf");

	m_pausedText.setFont(m_font);
	m_pausedText.setCharacterSize(155);
	m_pausedText.setFillColor(sf::Color::White);
	m_pausedText.setPosition(400, 400);
	m_pausedText.setString("Press Enter\nto continue");

	m_gameOverText.setFont(m_font);
	m_gameOverText.setCharacterSize(125);
	m_gameOverText.setFillColor(sf::Color::White);
	m_gameOverText.setPosition(250, 850);
	m_gameOverText.setString("Press Enter to play");

	m_ammoText.setFont(m_font);
	m_ammoText.setCharacterSize(55);
	m_ammoText.setFillColor(sf::Color::White);
	m_ammoText.setPosition(200, 980);

	m_scoreText.setFont(m_font);
	m_scoreText.setCharacterSize(55);
	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setPosition(20, 0);

	m_healthBar.setFillColor(sf::Color::Red);
	m_healthBar.setPosition(450, 980);
}


void HUD::update(float dt)
{
	m_healthBar.setSize(Vector2f(m_player.getHealth() * 3, 50));
}


void HUD::render(sf::RenderWindow& window) const
{
	window.draw(m_ammoText);
	window.draw(m_ammoIcon);
	window.draw(m_scoreText);
	window.draw(m_healthBar);
}
