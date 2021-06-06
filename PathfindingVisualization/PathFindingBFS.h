#pragma once

#include "Graph.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

class PathFindingBFS
{
public:	
	PathFindingBFS();

	void start(glm::ivec2 startPosition, glm::ivec2 endPosition);
	void update(std::vector<sf::RectangleShape>& map);

private:
	Graph m_graph;
	std::queue<glm::ivec2> m_frontier;
	glm::ivec2 m_startPosition;
	glm::ivec2 m_endPosition;
};