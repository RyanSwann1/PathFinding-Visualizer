#pragma once

#include "glm.hpp"
#include "glm/gtx/hash.hpp"
#include "Globals.h"
#include <unordered_map>
#include <vector>
#include <queue>

struct MinHeapAStarNode
{
	MinHeapAStarNode(glm::ivec2 position, float g, float h)
		: position(position),
		g(g),
		h(h)
	{}

	float getCost() const { return g + h; }

	glm::ivec2 position;
	float g;
	float h;
};

struct MinHeapThetaStarNode : public MinHeapAStarNode
{
	MinHeapThetaStarNode(glm::ivec2 position, glm::ivec2 parent, float g, float h)
		: MinHeapAStarNode(position, g, h),
		parent(parent)
	{}

	glm::ivec2 parent;
};

struct MinHeapDijkstraNode
{
	MinHeapDijkstraNode(glm::ivec2 position, glm::ivec2 cameFrom, int cost)
		: position(position),
		cameFrom(cameFrom),
		cost(cost)
	{}

	int getCost() const { return cost; }

	glm::ivec2 position;
	glm::ivec2 cameFrom;
	int cost;
};

template <typename Node>
class MinHeap
{
public:
	MinHeap()
		: m_heap(),
		m_indexes()
	{
		m_heap.reserve(Globals::MAP_SIZE.x * Globals::MAP_SIZE.y);
	}

	bool isEmpty() const
	{
		return m_heap.size() == 0;
	}

	void add(const Node& node)
	{
		m_heap.push_back(node);
		assert(m_indexes.find(m_heap.back().position) == m_indexes.cend());
		m_indexes.emplace(node.position, m_heap.size() - 1);
		sortBottomNode();
	}

	Node pop()
	{
		assert(!isEmpty());
		Node node = m_heap.front();
		findAndErase(node.position);

		return node;
	}

	bool findAndErase(glm::ivec2 position)
	{
		if (m_heap.size() > 1)
		{
			assert(m_indexes.size() > 1);
			auto iter = m_indexes.find(position);
			if (iter != m_indexes.end())
			{
				int index = iter->second;
				assert(index < m_heap.size() && iter->first == m_heap[index].position);
				swap(index, m_heap.size() - 1);

				iter = m_indexes.find(position);
				assert(iter != m_indexes.end());
				m_indexes.erase(iter);
				m_heap.pop_back();

				sortNode(index);
				return true;
			}
		}
		else if (m_heap.size() == 1)
		{
			assert(m_indexes.size() == 1);
			if (m_heap[0].position == position)
			{
				m_indexes.clear();
				m_heap.clear();

				return true;
			}
		}

		return false;
	}

	void clear()
	{
		m_heap.clear();
		m_indexes.clear();
	}

	
private:
	std::vector<Node> m_heap;
	std::unordered_map<glm::ivec2, int> m_indexes;
	
	void sortTopNode()
	{
		sortNode(0);
	}

	void sortBottomNode()
	{
		assert(!isEmpty());
		int index = m_heap.size() - 1;
		while (getParentIndex(index) >= 0 && m_heap[getParentIndex(index)].getCost() > m_heap[index].getCost())
		{
			swap(getParentIndex(index), index);
			index = getParentIndex(index);
		}
	}

	void sortNode(int i)
	{
		if (i < m_heap.size())
		{
			while (getLeftChildIndex(i) < m_heap.size())
			{
				int smallestChildIndex = getLeftChildIndex(i);
				if (getRightChildIndex(i) < m_heap.size() &&
					m_heap[getRightChildIndex(i)].getCost() < m_heap[getLeftChildIndex(i)].getCost())
				{
					smallestChildIndex = getRightChildIndex(i);
				}

				if (m_heap[smallestChildIndex].getCost() < m_heap[i].getCost())
				{
					swap(smallestChildIndex, i);
					i = smallestChildIndex;
				}
				else
				{
					break;
				}
			}
		}
	}

	void swap(int index1, int index2)
	{
		assert(index1 < m_heap.size() && index2 < m_heap.size());

		auto iter1 = m_indexes.find(m_heap[index1].position);
		assert(iter1 != m_indexes.end() && iter1->second == index1);

		auto iter2 = m_indexes.find(m_heap[index2].position);
		assert(iter2 != m_indexes.end() && iter2->second == index2);

		std::swap(iter1->second, iter2->second);
		std::swap(m_heap[index2], m_heap[index1]);
	}

	int getParentIndex(int index) const
	{
		return (index - 1) / 2;
	}

	int getLeftChildIndex(int index) const
	{
		return index * 2 + 1;
	}

	int getRightChildIndex(int index) const
	{
		return index * 2 + 2;
	}
};