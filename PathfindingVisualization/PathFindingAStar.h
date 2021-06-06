#pragma once

#include "glm.hpp"
#include "glm/gtx/hash.hpp"
#include "MinHeap.h"
#include <SFML/Graphics.hpp>
#include <vector>

struct AStarGraphNode 
{
	AStarGraphNode();
	AStarGraphNode(glm::ivec2 position, glm::ivec2 cameFrom, float g, float h);

	float getF() const;

	glm::ivec2 position;
	glm::ivec2 cameFrom;
	float g;
	float h;
};

class PathFindingAStar
{
public:
	PathFindingAStar(); 

	void start(glm::ivec2 startPosition, glm::ivec2 endPosition);
	void update(std::vector<sf::RectangleShape>& map);

private:
	MinHeap<MinHeapAStarNode> m_frontier;
	std::vector<AStarGraphNode> m_graph;
	glm::ivec2 m_startPosition;
	glm::ivec2 m_endPosition;
};