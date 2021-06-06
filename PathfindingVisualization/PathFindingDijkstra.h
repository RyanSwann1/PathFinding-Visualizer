#pragma once

#include "glm.hpp"
#include "MinHeap.h"
#include <SFML/Graphics.hpp>
#include <vector>

struct DijkstraGraphNode 
{
	DijkstraGraphNode(glm::ivec2 position);

	glm::ivec2 position;
	glm::ivec2 cameFrom;
	int cost;
	bool visited;
};

class PathFindingDijkstra
{
public:
	PathFindingDijkstra();

	void start(glm::ivec2 startPosition, glm::ivec2 endPosition);
	void update(std::vector<sf::RectangleShape>& map);

private:
	MinHeap<MinHeapDijkstraNode> m_frontier;
	std::vector<DijkstraGraphNode> m_graph;
	glm::ivec2 m_startPosition;
	glm::ivec2 m_endPosition;
};