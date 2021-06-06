#include "PathFindingAStar.h"
#include "AdjacentPositions.h"
#include "Globals.h"
#include "glm/gtx/norm.hpp"

namespace
{
	float getDistance(glm::ivec2 positionA, glm::ivec2 positionB)
	{	
		return glm::distance2(glm::vec2(positionA), glm::vec2(positionB));
	}
}

AStarGraphNode::AStarGraphNode()
	: position(0, 0),
	cameFrom(0, 0),
	g(0.f),
	h(0.f)
{}

AStarGraphNode::AStarGraphNode(glm::ivec2 position, glm::ivec2 cameFrom, float g, float h)
	: position(position),
	cameFrom(cameFrom),
	g(g),
	h(h)
{}

float AStarGraphNode::getF() const
{
	return g + h;
}

PathFindingAStar::PathFindingAStar()
	: m_frontier(),
	m_startPosition(0, 0),
	m_endPosition(0, 0)
{
	m_graph.resize(Globals::MAP_SIZE.x * Globals::MAP_SIZE.y);
}

void PathFindingAStar::start(glm::ivec2 startPosition, glm::ivec2 endPosition)
{
	std::fill(m_graph.begin(), m_graph.end(), AStarGraphNode());
	m_frontier.clear();
	m_frontier.add({startPosition, 0.0f, getDistance(endPosition, startPosition)});
	m_startPosition = startPosition;
	m_endPosition = endPosition;
}

void PathFindingAStar::update(std::vector<sf::RectangleShape>& map)
{
	if (!m_frontier.isEmpty())
	{
		MinHeapAStarNode currentNode = m_frontier.pop();
		map[Globals::convert2DTo1D(currentNode.position)].setFillColor(sf::Color::Blue);
		
		if (currentNode.position == m_endPosition)
		{
			glm::ivec2 position = currentNode.position;
			while (position != m_startPosition)
			{
				map[Globals::convert2DTo1D(position)].setFillColor(sf::Color::Yellow);
				position = m_graph[Globals::convert2DTo1D(position)].cameFrom;
			}

			map[Globals::convert2DTo1D(position)].setFillColor(sf::Color::Yellow);
			std::fill(m_graph.begin(), m_graph.end(), AStarGraphNode());
			m_frontier.clear();
			return;
		}

		for (auto adjacentPosition : getAdjacentPositions(currentNode.position, map))
		{
			if (adjacentPosition.valid)
			{
				float costFromStart = currentNode.g + getDistance(adjacentPosition.position, currentNode.position);
				if (m_graph[Globals::convert2DTo1D(adjacentPosition.position)].getF() > 0.0f)
				{
					AStarGraphNode& adjacentGraphNode = m_graph[Globals::convert2DTo1D(adjacentPosition.position)];
					if (costFromStart < adjacentGraphNode.g + getDistance(adjacentGraphNode.cameFrom, adjacentGraphNode.position)) //Weird line, need to fix o_o
					{
						adjacentGraphNode.g = costFromStart;
						adjacentGraphNode.cameFrom = currentNode.position;
						if (m_frontier.findAndErase(adjacentGraphNode.position))
						{
							m_frontier.add({ adjacentGraphNode.position, adjacentGraphNode.g, adjacentGraphNode.h });
						}
					}
				}
				else
				{
					float costFromEnd = getDistance(m_endPosition, adjacentPosition.position);
					m_frontier.add({ adjacentPosition.position, costFromStart, costFromEnd });
					m_graph[Globals::convert2DTo1D(adjacentPosition.position)] = 
						{ adjacentPosition.position, currentNode.position, costFromStart, costFromEnd };
				}
			}
		}
	}
}