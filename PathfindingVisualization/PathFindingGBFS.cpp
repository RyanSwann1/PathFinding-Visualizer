#include "PathFindingGBFS.h"
#include "Globals.h"
#include "AdjacentPositions.h"
#include <iostream>
#include "glm/gtx/norm.hpp"

namespace {
	float getDistance(glm::ivec2 target, glm::ivec2 position)
	{
		return glm::distance2(glm::vec2(target), glm::vec2(position));
	}
}

GBFSFrontierNode::GBFSFrontierNode(glm::ivec2 position, float distanceFromTarget)
	: position(position),
	distanceFromTarget(distanceFromTarget)
{}

PathFindingGBFS::PathFindingGBFS()
	: m_frontier(GBFSFrontierNodeComparator),
	m_graph(),
	m_startPosition(),
	m_endPosition()
{}

void PathFindingGBFS::start(glm::ivec2 startingPosition, glm::ivec2 targetPosition)
{
	m_graph.reset();
	m_startPosition = startingPosition;
	m_endPosition = targetPosition;
	m_frontier.emplace(startingPosition, getDistance(targetPosition, startingPosition));
}

void PathFindingGBFS::update(std::vector<sf::RectangleShape>& map)
{
	if(!m_frontier.empty())
	{
		glm::ivec2 position = m_frontier.top().position;
		m_frontier.pop();
		map[Globals::convert2DTo1D(position)].setFillColor(sf::Color::Yellow);
		if (position == m_endPosition)
		{
			while (!m_frontier.empty())
			{
				m_frontier.pop();
			}

			for (auto i : m_graph.getPath(m_startPosition, m_endPosition))
			{
				map[Globals::convert2DTo1D(i)].setFillColor(sf::Color::Green);
			}

			return;
		}

		for (auto adjacentPosition : getAdjacentPositions(position, map))
		{
			if (adjacentPosition.valid && !m_graph.isPositionAdded(adjacentPosition.position))
			{
				m_graph.addToGraph(adjacentPosition.position, position);
				m_frontier.emplace(adjacentPosition.position, getDistance(m_endPosition, adjacentPosition.position));
			}
		}
	}
}