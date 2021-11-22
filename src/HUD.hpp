#pragma once

class HUD
{
private:
	class Player& m_player;

	sf::Sprite m_gameOverSprite;
	sf::Sprite m_ammoIcon;

	sf::Font m_font;
	sf::Text m_pausedText;
	sf::Text m_gameOverText;
	sf::Text m_levelUpText;
	sf::Text m_ammoText;
	sf::Text m_scoreText;
	sf::Text m_highScoreText;
	sf::Text m_zombieRemainingText;
	sf::Text m_waveNumberText;

	sf::RectangleShape m_healthBar;

public:
	HUD(class Player& player);

	void update(float dt);

	void render(sf::RenderWindow& window) const;
};
