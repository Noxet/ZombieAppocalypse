#include "zpch.hpp"

#include "GamePlay.hpp"


int GamePlay::createBackground(sf::VertexArray& va, sf::IntRect arena)
{
	// size of each tile/texture
	constexpr int TILE_SIZE = 50;
	constexpr int TILE_TYPES = 4;
	constexpr int VERTS_IN_QUAD = 4;

	// width and height in number of tiles
	const int worldWidth = arena.width / TILE_SIZE;
	const int worldHeight = arena.height / TILE_SIZE;

	va.setPrimitiveType(sf::Quads);
	va.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

	int currentVertex = 0;

	for (int w = 0; w < worldWidth; ++w)
	{
		for (int h = 0; h < worldHeight; ++h)
		{
			// position each vertex in the current quad
			va[currentVertex].position = Vector2f(static_cast<float>(w * TILE_SIZE), static_cast<float>(h * TILE_SIZE));
			va[currentVertex + 1].position = Vector2f(static_cast<float>((w * TILE_SIZE) + TILE_SIZE), static_cast<float>(h * TILE_SIZE));
			va[currentVertex + 2].position = Vector2f(static_cast<float>((w * TILE_SIZE) + TILE_SIZE), static_cast<float>((h * TILE_SIZE) + TILE_SIZE));
			va[currentVertex + 3].position = Vector2f(static_cast<float>((w * TILE_SIZE)), static_cast<float>((h * TILE_SIZE) + TILE_SIZE));

			int tileType{};

			if (h == 0 || h == worldHeight - 1 || w == 0 || w == worldWidth - 1)
			{
				// set the borders to the wall texture
				tileType = TileType::Wall;
			}
			else
			{
				// randomize to any floor texture
				tileType = (rand() % (TILE_TYPES - 1));
			}

			va[currentVertex].texCoords = Vector2f(0, static_cast<float>(tileType * TILE_SIZE));
			va[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, static_cast<float>(tileType * TILE_SIZE));
			va[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, static_cast<float>(TILE_SIZE + tileType * TILE_SIZE));
			va[currentVertex + 3].texCoords = Vector2f(0, static_cast<float>(TILE_SIZE + tileType * TILE_SIZE));
			

			currentVertex += VERTS_IN_QUAD;
		}
	}

	return TILE_SIZE;
}
