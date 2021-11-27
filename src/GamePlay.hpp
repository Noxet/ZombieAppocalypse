#pragma once

#include "Zombie.hpp"

class GamePlay
{
private:
	enum TileType { GroundNormal, GroundGrass, GroundDirt, Wall };

public:
	inline static float sfxVolume{ 40.f };

	static int createBackground(sf::VertexArray& va, sf::IntRect arena);
	static std::vector<std::unique_ptr<Zombie>> createHorde(int numZombies, sf::IntRect arena);
};
