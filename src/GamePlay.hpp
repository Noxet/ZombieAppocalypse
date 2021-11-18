#pragma once

class GamePlay
{
private:
	enum TileType { GroundNormal, GroundGrass, GroundDirt, Wall };

public:
	static int createBackground(sf::VertexArray& va, sf::IntRect arena);
};