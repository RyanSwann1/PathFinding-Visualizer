#pragma once

#include "Graph.h"
#include <queue>
#include <SFML/Graphics.hpp>
#include <functional>

struct GBFSFrontierNode
{
	GBFSFrontierNode(glm::ivec2 position, float distanceFromTarget);

	glm::ivec2 position;
	float distanceFromTarget;
};

auto GBFSFrontierNodeComparator = [](const auto& a, const auto& b) -> bool { return b.distanceFromTarget < a.distanceFromTarget; };
class PathFindingGBFS
{
public:
	PathFindingGBFS();

	void start(glm::ivec2 startingPosition, glm::ivec2 targetPosition);
	void update(std::vector<sf::RectangleShape>& map);

private:
	std::priority_queue<GBFSFrontierNode, 
		std::vector<GBFSFrontierNode>, decltype(GBFSFrontierNodeComparator)> m_frontier;
	Graph m_graph;
	glm::ivec2 m_startPosition;
	glm::ivec2 m_endPosition;
};