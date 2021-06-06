#pragma once

#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include <random>

namespace Globals
{
	const glm::ivec2 MAP_SIZE = { 90, 40 };
	const int GRID_SIZE = 16;
	const sf::Color EXPLORED_MAP_TILE_COLOUR = sf::Color::Green;
	const sf::Color WALL = sf::Color::Red;

	inline bool isWithinMapBounds(glm::ivec2 position)
	{
		return position.x >= 0 && position.x < MAP_SIZE.x &&
			position.y >= 0 && position.y < MAP_SIZE.y;
	}
	
	inline int convert2DTo1D(glm::ivec2 position)
	{
		return position.y * MAP_SIZE.x + position.x;
	}


}