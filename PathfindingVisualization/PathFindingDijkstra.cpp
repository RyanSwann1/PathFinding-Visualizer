#include "PathFindingDijkstra.h"
#include "AdjacentPositions.h"
#include "Globals.h"
#include <assert.h>

DijkstraGraphNode::DijkstraGraphNode(glm::ivec2 position)
	: position(position),
	cameFrom(0, 0),
	cost(0),
	visited(false)
{}

PathFindingDijkstra::PathFindingDijkstra()
	: m_frontier(),
	m_startPosition(0, 0),
	m_endPosition(0, 0)
{
	m_graph.reserve(Globals::MAP_SIZE.x * Globals::MAP_SIZE.y);
}

void PathFindingDijkstra::start(glm::ivec2 startPosition, glm::ivec2 endPosition)
{	
	for (int y = 0; y < Globals::MAP_SIZE.y; ++y)
	{
		for (int x = 0; x < Globals::MAP_SIZE.x; ++x)
		{
			m_graph.emplace_back(glm::ivec2(x, y));
		}
	}
	m_frontier.clear();
	m_frontier.add({ startPosition, startPosition, 0 });
	m_startPosition = startPosition;
	m_endPosition = endPosition;
}

void PathFindingDijkstra::update(std::vector<sf::RectangleShape>& map)
{
	if (!m_frontier.isEmpty())
	{
		MinHeapDijkstraNode currentNode = m_frontier.pop();
		map[Globals::convert2DTo1D(currentNode.position)].setFillColor(sf::Color::Blue);
		if (currentNode.position == m_endPosition)
		{
			glm::ivec2 position = currentNode.position;
			while (position != m_startPosition)
			{
				map[Globals::convert2DTo1D(position)].setFillColor(sf::Color::Yellow);
				position = m_graph[Globals::convert2DTo1D(position)].cameFrom;
			}

			m_frontier.clear();
			m_graph.clear();

			return;
		}

		for (auto adjacentPosition : getSmallerWeightedAdjacentPositions(currentNode.position, map))
		{
			if (adjacentPosition.valid)
			{
				DijkstraGraphNode& adjacentGraphNode = m_graph[Globals::convert2DTo1D(adjacentPosition.position)];
				const DijkstraGraphNode& currentGraphNode = m_graph[Globals::convert2DTo1D(currentNode.position)];
				if (adjacentGraphNode.visited)
				{
					const DijkstraGraphNode& cameFromAdjacentGraphNode = m_graph[Globals::convert2DTo1D(adjacentGraphNode.cameFrom)];
					if (currentGraphNode.cost + adjacentPosition.weight < cameFromAdjacentGraphNode.cost + adjacentPosition.weight)
					{
						assert(currentGraphNode.position != cameFromAdjacentGraphNode.position);
						adjacentGraphNode.cost = currentGraphNode.cost + adjacentPosition.weight;
						adjacentGraphNode.cameFrom = currentNode.position;
						
						if (m_frontier.findAndErase(adjacentPosition.position))
						{
							m_frontier.add({adjacentPosition.position, currentNode.position, currentGraphNode.cost + adjacentPosition.weight});
						}
					}
				}
				else
				{
					adjacentGraphNode.visited = true;
					adjacentGraphNode.cameFrom = currentNode.position;
					adjacentGraphNode.cost = currentGraphNode.cost + adjacentPosition.weight;
				
					m_frontier.add({adjacentPosition.position, currentNode.position, currentGraphNode.cost + adjacentPosition.weight});
				}
			}
		}
	}
}