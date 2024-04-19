#include <iostream>
#include <string>
#include "SFML/Graphics.hpp"

#include "EntityManager.h"
#include "Component.h"


EntityManager::EntityManager() {};

EntityManager::EntityManager(int row, int col, int size, sf::Font& font)
{
	// init debug grid
	for (int r = 0; r < row; r++)
		for (int c = 0; c < col; c++)
		{
			auto shape = sf::RectangleShape(sf::Vector2f(size, size));
			shape.setPosition(sf::Vector2f(c*size, r*size));
			shape.setFillColor(sf::Color(0, 0, 0, 0));
			shape.setOutlineColor(sf::Color::White);
			shape.setOutlineThickness(-1);
			std::get<std::vector<sf::RectangleShape>>(m_board).push_back(shape);

			auto text = sf::Text::Text();
			text.setFont(font);
			text.setString(std::to_string(r) + "," + std::to_string(c));
			text.setPosition(sf::Vector2f(c * size + 2, r * size + 2));
			text.setCharacterSize(10);
			std::get<std::vector<sf::Text>>(m_board).push_back(text);
		}
}

int EntityManager::addEntity(cPosition p, cShape s, cMovable m, cLifetime t)
{ 
	m_id++;
	std::get<std::vector<cPosition>>(m_blocks).push_back(p);
	std::get<std::vector<cShape>>(m_blocks).push_back(s);
	std::get<std::vector<cMovable>>(m_blocks).push_back(m);
	std::get<std::vector<cLifetime>>(m_blocks).push_back(t);
	m_entitiesID.push_back(m_id);
	return m_id; 
}

void EntityManager::removeEntity(int id) 
{
	for (size_t i = 0; i < m_entitiesID.size(); i++)
	{
		if (m_entitiesID[i] == id)
		{
			m_entitiesID.erase(m_entitiesID.begin() + i);
			removeComponent<cPosition>(i);
			removeComponent<cShape>(i);
			removeComponent<cMovable>(i);
			removeComponent<cLifetime>(i);
			break;
		}
	}
}


std::vector<int>& EntityManager::getIDs() 
{
	return m_entitiesID;
}


void EntityManager::clear()
{
	std::get<std::vector<cPosition>>(m_blocks).clear();
	std::get<std::vector<cShape>>(m_blocks).clear();
	std::get<std::vector<cMovable>>(m_blocks).clear();
	std::get<std::vector<cLifetime>>(m_blocks).clear();
	m_entitiesID.clear();
}