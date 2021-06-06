#pragma once

#include "glm/glm.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>

extern const std::array<glm::ivec2, 8> ALL_DIRECTIONS_ON_GRID;
extern const std::array<glm::ivec2, 4> DIRECTIONS_ON_GRID;

struct AdjacentPosition
{
	AdjacentPosition();
	AdjacentPosition(glm::ivec2 position);
	AdjacentPosition(glm::ivec2 position, int weight);

	glm::ivec2 position;
	bool valid;
	int weight;
};

std::array<AdjacentPosition, 8> getAdjacentPositions(glm::ivec2 position, const std::vector<sf::RectangleShape>& map);
std::array<AdjacentPosition, 8> getWeightedAdjacentPositions(glm::ivec2 position, const std::vector<sf::RectangleShape>& map);
std::array<AdjacentPosition, 4> getSmallerWeightedAdjacentPositions(glm::ivec2 position, const std::vector<sf::RectangleShape>& map);