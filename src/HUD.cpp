#include "zpch.hpp"

#include "HUD.hpp"
#include "Player.hpp"
#include <sstream>


/**
 * A static overlay to show stats
 */
HUD::HUD(Player& player, int& score, int& highScore, int& numZombiesAlive) :
	m_player(player), m_score(score), m_highScore(highScore), m_numZombiesAlive(numZombiesAlive)
{
	m_ammoIcon.setTexture(TextureHolder::getTexture("../assets/gfx/ammo_icon.png"));
	m_ammoIcon.setPosition(20, 1100);

	m_font.loadFromFile("../assets/fonts/zombiecontrol.ttf");

	m_ammoText.setFont(m_font);
	m_ammoText.setCharacterSize(55);
	m_ammoText.setFillColor(sf::Color::White);
	m_ammoText.setPosition(200, 1100);

	m_scoreText.setFont(m_font);
	m_scoreText.setCharacterSize(55);
	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setPosition(20, 0);

	m_healthBar.setFillColor(sf::Color::Red);
	m_healthBar.setPosition(450, 1100);

	m_zombieRemainingText.setFont(m_font);
	m_zombieRemainingText.setCharacterSize(55);
	m_zombieRemainingText.setFillColor(sf::Color::White);
	m_zombieRemainingText.setPosition(1500, 1100);
}


void HUD::update(float dt)
{
	m_healthBar.setSize(Vector2f(m_player.getHealth() * 3, 50));
}


void HUD::render(sf::RenderWindow& window)
{
	window.draw(m_ammoIcon);

	std::stringstream ssAmmo;
	ssAmmo << m_player.getBulletsInClip() << "/" << m_player.getBulletsSpare();
	m_ammoText.setString(ssAmmo.str());
	window.draw(m_ammoText);

	std::stringstream ssScore;
	ssScore << "Score: " << m_score;
	m_scoreText.setString(ssScore.str());
	window.draw(m_scoreText);

	std::stringstream ssHighScore;
	ssHighScore << "High score: " << m_highScore;
	m_highScoreText.setString(ssHighScore.str());
	window.draw(m_highScoreText);

	std::stringstream ssZombiesAlive;
	ssZombiesAlive << "Zombies: " << m_numZombiesAlive;
	m_zombieRemainingText.setString(ssZombiesAlive.str());
	window.draw(m_zombieRemainingText);

	window.draw(m_healthBar);
}
