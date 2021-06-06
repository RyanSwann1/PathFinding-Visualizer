#pragma once

#include "SFML/Graphics.hpp"
#include "glm/glm.hpp"
#include <vector>

struct GraphNode
{
	GraphNode();
	GraphNode(glm::ivec2 cameFrom);

	bool visited;
	glm::ivec2 cameFrom;
};

class Graph
{
public:
	Graph();

	std::vector<glm::ivec2> getPath(glm::ivec2 startPosition, glm::ivec2 endPosition) const;
	bool isPositionAdded(glm::ivec2 position) const;

	void addToGraph(glm::ivec2 position, glm::ivec2 cameFrom);
	void reset();

private:
	std::vector<GraphNode> m_graph;
};