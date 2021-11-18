#pragma once

#include "Zombie.hpp"

class GamePlay
{
private:
	enum TileType { GroundNormal, GroundGrass, GroundDirt, Wall };

public:
	static int createBackground(sf::VertexArray& va, sf::IntRect arena);
	static std::vector<std::unique_ptr<Zombie>> createHorde(int numZombies, sf::IntRect arena);
};