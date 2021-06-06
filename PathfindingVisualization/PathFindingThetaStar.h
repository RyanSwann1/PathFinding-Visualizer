#pragma once

#include "glm.hpp"
#include "MinHeap.h"
#include <SFML/Graphics.hpp>
#include <vector>

struct ThetaStarGraphNode
{
	ThetaStarGraphNode();
	ThetaStarGraphNode(glm::ivec2 position, glm::ivec2 cameFrom, float g, float h);

	float getF() const;

	glm::ivec2 position;
	glm::ivec2 cameFrom;
	float g;
	float h;
};

class PathFindingThetaStar
{
public:
	PathFindingThetaStar();

	void start(glm::ivec2 startPosition, glm::ivec2 endPosition);
	void update(std::vector<sf::RectangleShape>& map);

private:
	MinHeap<MinHeapThetaStarNode> m_frontier;
	std::vector<ThetaStarGraphNode> m_graph;
	glm::ivec2 m_startPosition;
	glm::ivec2 m_endPosition;
};