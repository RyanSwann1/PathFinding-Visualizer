#include "AdjacentPositions.h"
#include "Globals.h"

namespace
{
	const int MIN_WEIGHT = 1;
	const int MAX_WEIGHT = 250;
}

const std::array<glm::ivec2, 8> ALL_DIRECTIONS_ON_GRID =
{
	glm::ivec2(0, 1),
	glm::ivec2(1, 1),
	glm::ivec2(1, 0),
	glm::ivec2(1, -1),
	glm::ivec2(0, -1),
	glm::ivec2(-1, -1),
	glm::ivec2(-1, 0),
	glm::ivec2(-1, 1)
};

const std::array<glm::ivec2, 4> DIRECTIONS_ON_GRID =
{
	glm::ivec2(0, 1),
	glm::ivec2(1, 0),
	glm::ivec2(0, -1),
	glm::ivec2(-1, 0)
};

AdjacentPosition::AdjacentPosition()
	: position(0, 0),
	valid(false),
	weight(MIN_WEIGHT)
{}

AdjacentPosition::AdjacentPosition(glm::ivec2 position)
	: position(position),
	valid(true),
	weight(MIN_WEIGHT)
{}

AdjacentPosition::AdjacentPosition(glm::ivec2 position, int weight)
	: position(position),
	valid(true),
	weight(weight)
{}

std::array<AdjacentPosition, 8> getWeightedAdjacentPositions(glm::ivec2 position, const std::vector<sf::RectangleShape>& map)
{
	std::array<AdjacentPosition, 8> adjacentPositions;
	for (int i = 0; i < ALL_DIRECTIONS_ON_GRID.size(); ++i)
	{
		glm::ivec2 adjacentPosition = position + ALL_DIRECTIONS_ON_GRID[i];
		if (Globals::isWithinMapBounds(adjacentPosition))
		{
			if (map[Globals::convert2DTo1D(adjacentPosition)].getFillColor() == Globals::WALL)
			{
				adjacentPositions[i] = AdjacentPosition(adjacentPosition, MAX_WEIGHT);
			}
			else
			{
				adjacentPositions[i] = AdjacentPosition(adjacentPosition);
			}
		}
	}

	return adjacentPositions;
}

std::array<AdjacentPosition, 4> getSmallerWeightedAdjacentPositions(glm::ivec2 position, const std::vector<sf::RectangleShape>& map)
{
	std::array<AdjacentPosition, 4> adjacentPositions;
	for (int i = 0; i < DIRECTIONS_ON_GRID.size(); ++i)
	{
		glm::ivec2 adjacentPosition = position + DIRECTIONS_ON_GRID[i];
		if (Globals::isWithinMapBounds(adjacentPosition))
		{
			if (map[Globals::convert2DTo1D(adjacentPosition)].getFillColor() == Globals::WALL)
			{
				adjacentPositions[i] = AdjacentPosition(adjacentPosition, MAX_WEIGHT);
			}
			else
			{
				adjacentPositions[i] = AdjacentPosition(adjacentPosition);
			}
		}
	}

	return adjacentPositions;
}

std::array<AdjacentPosition, 8> getAdjacentPositions(glm::ivec2 position, const std::vector<sf::RectangleShape>& map)
{
	std::array<AdjacentPosition, 8> adjacentPositions;
	for (int i = 0; i < ALL_DIRECTIONS_ON_GRID.size(); ++i)
	{
		glm::ivec2 adjacentPosition = position + ALL_DIRECTIONS_ON_GRID[i];
		if (Globals::isWithinMapBounds(adjacentPosition) && 
			map[Globals::convert2DTo1D(adjacentPosition)].getFillColor() != Globals::WALL)
		{
			adjacentPositions[i] = AdjacentPosition(adjacentPosition);
		}
	}

	return adjacentPositions;
}