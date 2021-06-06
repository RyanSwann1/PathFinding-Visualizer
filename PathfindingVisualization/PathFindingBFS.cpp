#include "PathFindingBFS.h"
#include "Globals.h"
#include "AdjacentPositions.h"

PathFindingBFS::PathFindingBFS()
	: m_graph(),
	m_frontier(),
	m_endPosition(0, 0)
{}

void PathFindingBFS::start(glm::ivec2 startPosition, glm::ivec2 endPosition)
{
	m_graph.reset();
	m_frontier.push(startPosition);
	m_startPosition = startPosition;
	m_endPosition = endPosition;
}

void PathFindingBFS::update(std::vector<sf::RectangleShape>& map)
{
	if (!m_frontier.empty())
	{
		glm::ivec2 position = m_frontier.front();
		m_frontier.pop();
		if (position == m_endPosition)
		{
			m_frontier = std::queue<glm::ivec2>();
			for (auto i : m_graph.getPath(m_startPosition, m_endPosition))
			{
				map[Globals::convert2DTo1D(i)].setFillColor(sf::Color::Green);
			}
			map[Globals::convert2DTo1D(position)].setFillColor(sf::Color::Blue);
			return;
		}
		else
		{
			map[Globals::convert2DTo1D(position)].setFillColor(sf::Color::Yellow);
		}

		for (auto adjacentPosition : getAdjacentPositions(position, map))
		{
			if (adjacentPosition.valid && !m_graph.isPositionAdded(adjacentPosition.position))
			{
				m_graph.addToGraph(adjacentPosition.position, position);
				m_frontier.push(adjacentPosition.position);
			}
		}
	}
}