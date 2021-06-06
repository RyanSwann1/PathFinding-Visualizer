#include "Graph.h"
#include "Globals.h"
#include <assert.h>
#include <iostream>

GraphNode::GraphNode()
	: visited(false),
	cameFrom(0, 0)
{}

GraphNode::GraphNode(glm::ivec2 cameFrom)
	: visited(true),
	cameFrom(cameFrom)
{}

Graph::Graph()
{
	m_graph.resize(Globals::MAP_SIZE.x * Globals::MAP_SIZE.y);
}

std::vector<glm::ivec2> Graph::getPath(glm::ivec2 startPosition, glm::ivec2 endPosition) const
{
	assert(Globals::convert2DTo1D(startPosition) < m_graph.size() && Globals::convert2DTo1D(endPosition) < m_graph.size());
	std::vector<glm::ivec2> path;
	if (m_graph[Globals::convert2DTo1D(startPosition)].visited && m_graph[Globals::convert2DTo1D(endPosition)].visited)
	{
		glm::ivec2 currentPosition = endPosition;
		while (currentPosition != startPosition)
		{
			assert(m_graph[Globals::convert2DTo1D(currentPosition)].visited);
			path.push_back(currentPosition);
			currentPosition = m_graph[Globals::convert2DTo1D(currentPosition)].cameFrom;
		}
	}

	return path;
}

bool Graph::isPositionAdded(glm::ivec2 position) const
{
	assert(Globals::convert2DTo1D(position) < m_graph.size());
	return m_graph[Globals::convert2DTo1D(position)].visited;
}

void Graph::addToGraph(glm::ivec2 position, glm::ivec2 cameFrom)
{
	assert(Globals::convert2DTo1D(position) < m_graph.size() && 
		!m_graph[Globals::convert2DTo1D(position)].visited);

	m_graph[Globals::convert2DTo1D(position)] = GraphNode(cameFrom);
}

void Graph::reset()
{
	std::fill(m_graph.begin(), m_graph.end(), GraphNode());
}