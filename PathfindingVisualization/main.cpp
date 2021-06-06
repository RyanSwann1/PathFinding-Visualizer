#include <iostream>
#include "SFML/Window.hpp"
#include "Globals.h"
#include "PathFindingBFS.h"
#include "PathFindingGBFS.h"
#include "PathFindingDijkstra.h"
#include "PathFindingAStar.h"
#include "PathFindingThetaStar.h"

void resetMapNonWalls(std::vector<sf::RectangleShape>& map)
{
	for (auto& i : map)
	{
		if (i.getFillColor() != sf::Color::Red)
		{
			i.setFillColor(sf::Color::White);
		}
	}
}

void resetMap(std::vector<sf::RectangleShape>& map)
{
	for (auto& i : map)
	{
		i.setFillColor(sf::Color::White);
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1600, 800), "Pathfinding Stuff", sf::Style::Default);
	window.setFramerateLimit(200);
	std::vector<sf::RectangleShape> map(Globals::MAP_SIZE.x * Globals::MAP_SIZE.y);
	for (int y = 0; y < Globals::MAP_SIZE.y; ++y)
	{
		for (int x = 0; x < Globals::MAP_SIZE.x; ++x)
		{
			sf::RectangleShape& shape = map[Globals::convert2DTo1D({x, y})];
			shape.setPosition(x * Globals::GRID_SIZE, y * Globals::GRID_SIZE);
			shape.setSize({Globals::GRID_SIZE, Globals::GRID_SIZE});
			shape.setFillColor(sf::Color::White);
			shape.setOutlineColor(sf::Color::Black);
			shape.setOutlineThickness(2.0f);
		}
	}

	bool leftClickHeld = false;
	PathFindingBFS BFS;
	PathFindingGBFS GBFS;
	PathFindingDijkstra DIJKSTRA;
	PathFindingAStar ASTAR;
	PathFindingThetaStar THETASTAR;

	while (window.isOpen())
	{
		sf::Event currentSFMLEvent;
		while (window.pollEvent(currentSFMLEvent))
		{
			if (currentSFMLEvent.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (currentSFMLEvent.type == sf::Event::MouseButtonPressed)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					leftClickHeld = true;					
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
				{
					glm::ivec2 mousePosition =
					{ sf::Mouse::getPosition(window).x / Globals::GRID_SIZE,
					sf::Mouse::getPosition(window).y / Globals::GRID_SIZE };

					//BFS.start(mousePosition, { 0, 0 });
					//GBFS.start(mousePosition, {3, 3});
					//DIJKSTRA.start(mousePosition, { 3, 3 });
					//ASTAR.start(mousePosition, {3, 3});
					THETASTAR.start(mousePosition, {3, 3});
					if (mousePosition.x * mousePosition.y < map.size())
					{
						map[mousePosition.y * Globals::MAP_SIZE.x + mousePosition.x].setFillColor(sf::Color::Green);
					}
				}
			}
			else if (currentSFMLEvent.type == sf::Event::MouseButtonReleased)
			{
				leftClickHeld = false;
			}
			else if (currentSFMLEvent.type == sf::Event::KeyPressed)
			{
				if (currentSFMLEvent.key.code == sf::Keyboard::R)
				{
					resetMapNonWalls(map);
				}
			}
		}

		if (leftClickHeld)
		{
			glm::ivec2 mousePosition =
			{ sf::Mouse::getPosition(window).x / Globals::GRID_SIZE,
			sf::Mouse::getPosition(window).y / Globals::GRID_SIZE };

			if (mousePosition.x * mousePosition.y < map.size())
			{
				map[mousePosition.y * Globals::MAP_SIZE.x + mousePosition.x].setFillColor(Globals::WALL);
			}
		}

		BFS.update(map);
		GBFS.update(map);
		DIJKSTRA.update(map);
		ASTAR.update(map);
		THETASTAR.update(map);

		for (const auto& shape : map)
		{
			window.draw(shape);
		}

		window.display();
	}

	return 0;
}