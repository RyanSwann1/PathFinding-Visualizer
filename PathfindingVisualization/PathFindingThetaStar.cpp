#include "PathFindingThetaStar.h"
#include "AdjacentPositions.h"
#include "Globals.h"
#include "glm/gtx/norm.hpp"
#include <cmath>

namespace
{
	float getDistance(glm::ivec2 positionA, glm::ivec2 positionB)
	{
		return glm::distance(glm::vec2(positionA), glm::vec2(positionB));
	}

	glm::vec2 convertToGlobalCoords(glm::ivec2 position)
	{
		return { (position.x * Globals::GRID_SIZE) + Globals::GRID_SIZE / 2, (position.y * Globals::GRID_SIZE) + Globals::GRID_SIZE / 2 };
	}

	glm::ivec2 convertToGridCoords(glm::vec2 position)
	{
		return { position.x / Globals::GRID_SIZE, position.y / Globals::GRID_SIZE };
	}

	bool isInBounds(const sf::RectangleShape& rect, glm::vec2 position)
	{
		if (rect.getFillColor() == sf::Color::Red)
		{
			if (position.x >= rect.getPosition().x && position.x <= rect.getPosition().x + rect.getSize().x &&
				position.y >= rect.getPosition().y && position.y <= rect.getPosition().y + rect.getSize().y)
			{
				return true;
			}
		}

		return false;
	}

	//Bit hacky
	bool isInLineOfSight(const std::vector<sf::RectangleShape>& map, glm::ivec2 positionB, glm::ivec2 positionA)
	{
		glm::vec2 worldPositionA = convertToGlobalCoords(positionA);
		glm::vec2 worldPositionB = convertToGlobalCoords(positionB);
		glm::vec2 direction = glm::normalize(worldPositionB - worldPositionA);
		for (int i = 1; i <= std::ceil(glm::distance(worldPositionB, worldPositionA)); i += Globals::GRID_SIZE / 2)
		{
			glm::vec2 position = worldPositionA + direction * static_cast<float>(i);
			if (isInBounds(map[Globals::convert2DTo1D(convertToGridCoords(position))], position))
			{
				return false;
			}
		}

		return true;
	}
}

ThetaStarGraphNode::ThetaStarGraphNode()
	: position(0, 0),
	cameFrom(0, 0),
	g(0.f),
	h(0.f)
{}

ThetaStarGraphNode::ThetaStarGraphNode(glm::ivec2 position, glm::ivec2 cameFrom, float g, float h)
	: position(position),
	cameFrom(cameFrom),
	g(g),
	h(h)
{}

float ThetaStarGraphNode::getF() const
{
	return g + h;
}

PathFindingThetaStar::PathFindingThetaStar()
	: m_frontier(),
	m_startPosition(0, 0),
	m_endPosition(0, 0)
{
	m_graph.resize(Globals::MAP_SIZE.x * Globals::MAP_SIZE.y);
}

void PathFindingThetaStar::start(glm::ivec2 startPosition, glm::ivec2 endPosition)
{
	std::fill(m_graph.begin(), m_graph.end(), ThetaStarGraphNode());
	m_frontier.clear();
	m_frontier.add({startPosition, startPosition, 0.0f, getDistance(endPosition, startPosition)});
	m_startPosition = startPosition;
	m_endPosition = endPosition;
}

void PathFindingThetaStar::update(std::vector<sf::RectangleShape>& map)
{
	if (!m_frontier.isEmpty())
	{
		MinHeapThetaStarNode currentNode = m_frontier.pop();
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
			std::fill(m_graph.begin(), m_graph.end(), ThetaStarGraphNode());
			m_frontier.clear();
			return;
		}

		for (auto adjacentPosition : getAdjacentPositions(currentNode.position, map))
		{
			if (adjacentPosition.valid)
			{
				if (m_graph[Globals::convert2DTo1D(adjacentPosition.position)].getF() == 0.f)
				{
					float costFromStart = 0.f;
					float costFromEnd = getDistance(m_endPosition, adjacentPosition.position);
					glm::ivec2 cameFrom(0, 0);
					if (isInLineOfSight(map, adjacentPosition.position, currentNode.parent))
					{
						costFromStart = m_graph[Globals::convert2DTo1D(currentNode.parent)].g + getDistance(adjacentPosition.position, currentNode.parent);
						cameFrom = currentNode.parent;
					}
					else
					{
						costFromStart = currentNode.g + getDistance(adjacentPosition.position, currentNode.position);
						cameFrom = currentNode.position;
					}

					m_frontier.add({ adjacentPosition.position, cameFrom, costFromStart, costFromEnd });
					m_graph[Globals::convert2DTo1D(adjacentPosition.position)] =
						{ adjacentPosition.position, cameFrom, costFromStart, costFromEnd };
				}
				else
				{
					ThetaStarGraphNode& adjacentGraphNode = m_graph[Globals::convert2DTo1D(adjacentPosition.position)];
					float costFromStart = 0.f;
					glm::ivec2 cameFrom(0, 0);
					float cameFromG = 0.f;
					if (isInLineOfSight(map, adjacentPosition.position, currentNode.parent))
					{
						const ThetaStarGraphNode& parentGraphNode = m_graph[Globals::convert2DTo1D(currentNode.parent)];
						costFromStart = parentGraphNode.g + getDistance(adjacentPosition.position, parentGraphNode.position);
						cameFrom = parentGraphNode.position;
						cameFromG = parentGraphNode.g;
					}
					else
					{
						costFromStart = currentNode.g + getDistance(adjacentPosition.position, currentNode.position);
						cameFrom = currentNode.position;
						cameFromG = currentNode.g;
					}

					if (costFromStart < m_graph[Globals::convert2DTo1D(adjacentPosition.position)].g)
					{
						adjacentGraphNode.g = costFromStart;
						adjacentGraphNode.cameFrom = currentNode.position;
						if (m_frontier.findAndErase(adjacentPosition.position))
						{
							m_frontier.add({ adjacentPosition.position,
							cameFrom,
							cameFromG + getDistance(adjacentPosition.position, cameFrom),
							adjacentGraphNode.h});
						}
					}
				}
			}
		}
	}
}