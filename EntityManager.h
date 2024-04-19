#pragma once
#include <vector>
#include "SFML/Graphics.hpp"
#include "Component.h"

typedef std::tuple<
	std::vector<sf::RectangleShape>, 
	std::vector<sf::Text>
> BoardTuple;

typedef std::tuple<
	std::vector<cPosition>,
	std::vector<cShape>, 
	std::vector<cMovable>, 
	std::vector<cLifetime>
> BlockTuple;


class EntityManager
{
public:
	EntityManager();

	EntityManager(int row, int col, int size, sf::Font& font);

	int addEntity(cPosition p, cShape s, cMovable m, cLifetime t);

	template <typename T>
	T& getComponent(int id);

	template <typename T>
	std::vector<T>& getBoardComponentVec();

	template <typename T>
	std::vector<T>& getBlockComponentVec();

	std::vector<int>& getIDs();

	void removeEntity(int id);
	void clear();

private:
	int m_id = 0;

	std::vector<int> m_entitiesID;

	template <typename T>
	void removeComponent(int idx);

	BlockTuple m_blocks;
	BoardTuple m_board;
};


template <typename T>
T& EntityManager::getComponent(int id)
{
	return std::get<std::vector<T>>(m_blocks)[id];
}


template <typename T>
std::vector<T>& EntityManager::getBoardComponentVec()
{
	return std::get<std::vector<T>>(m_board);
}


template <typename T>
std::vector<T>& EntityManager::getBlockComponentVec()
{
	return std::get<std::vector<T>>(m_blocks);
}


template <typename T>
void EntityManager::removeComponent(int idx)
{
	std::get<std::vector<T>>(m_blocks).erase(std::get<std::vector<T>>(m_blocks).begin() + idx);
}