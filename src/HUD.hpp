#pragma once

class HUD
{
private:
	// references to "game engine" variables to be able to update the HUD
	class Player& m_player;
	int& m_score;
	int& m_highScore;
	int& m_numZombiesAlive;

	sf::Sprite m_gameOverSprite;
	sf::Sprite m_ammoIcon;

	sf::Font m_font;
	sf::Text m_ammoText;
	sf::Text m_scoreText;
	sf::Text m_highScoreText;
	sf::Text m_zombieRemainingText;
	sf::Text m_waveNumberText;

	sf::RectangleShape m_healthBar;

public:
	HUD(class Player& player, int& score, int& highScore, int& numZombiesAlive);

	void update(float dt);

	void render(sf::RenderWindow& window);
};
