#include "SpawnSystem.h"
#include "SFML/Graphics.hpp"



SpawnSystem::SpawnSystem() {}


void SpawnSystem::update(std::shared_ptr<EntityManager> entityManager, int nTileRow, int nTileCol, int gridSize, int lifetime, bool& spawn, bool& isPause, bool& isEnd)
{
	if (spawn)
	{
		spawnBlock(blockArr[std::rand() % 7], entityManager, nTileRow, nTileCol, gridSize, lifetime, spawn, isPause, isEnd);
	}
}


bool SpawnSystem::isSpawnVaild(std::shared_ptr<EntityManager> entityManager, std::vector<std::vector<int>>&c)
{
	for (std::vector<int> point : c)
	{
		int row = point[0];
		int col = point[1];

		for (auto& b : entityManager->getBlockComponentVec<cPosition>())
		{
			if (row == b.row && col == b.col)
			{
				return false;
			}
		}
	}
	return true;
}


void SpawnSystem::spawnBlock(char c, std::shared_ptr<EntityManager> entityManager, int nTileRow, int nTileCol, int gridSize, int lifetime, bool& spawn, bool& isPause, bool& isEnd)
{
	spawn = false;
	int row = 0;
	int col = (int)nTileCol / 2 - 1;
	int oRow = 0;
	int oCol = 0;
	std::vector<std::vector<int>> shape;
	sf::Color color;
	switch (c)
	{
	case 'O':
		shape = {
			{0, col}, {0, col + 1},
			{1, col}, {1, col + 1}
		};
		color = sf::Color::Red;
		oRow = 1;
		oCol = col + 1;
		break;
	case 'L':
		shape = {
			{0, col - 1}, {1, col - 1},
			{1, col}, {1, col + 1}
		};
		oRow = 1;
		oCol = col;
		color = sf::Color::Green;
		break;
	case 'I':
		shape = {
			{0, col}, {1, col},
			{2, col}, {3, col}
		};
		color = sf::Color::Blue;
		oRow = 2;
		oCol = col;
		break;
	case 'N':
		shape = {
			{0, col - 1}, {0, col},
			{1, col}, {1, col + 1}
		};
		color = sf::Color::Cyan;
		oRow = 1;
		oCol = col + 1;
		break;
	case 'Z':
		shape = {
			{0, col}, {0, col + 1},
			{1, col - 1}, {1, col}
		};
		color = sf::Color::Yellow;
		oRow = 1;
		oCol = col;
		break;
	case 'T':
		shape = {
			{0, col - 1}, {0, col},
			{0, col + 1}, {1, col}
		};
		color = sf::Color(245, 141, 66);
		oRow = 1;
		oCol = col + 1;
		break;
	case 'J':
		shape = {
			{0, col - 1}, {0, col},
			{0, col + 1}, {1, col - 1}
		};
		oRow = 1;
		oCol = col;
		color = sf::Color(66, 141, 245);
		break;
	default:
		break;
	}

	if (isSpawnVaild(entityManager, shape))
	{
		// gen new block and add it to entityManager
		for (auto& block : shape)
		{
			entityManager->addEntity(
				cPosition(block[0], block[1], oRow, oCol),
				cShape((float)block[1], (float)block[0], (int)gridSize, color),
				cMovable(true),
				cLifetime(lifetime)
			);
		}
	}
	else
	{
		for (auto& block : shape)
		{
			entityManager->addEntity(
				cPosition(block[0], block[1], oRow, oCol),
				cShape((float)block[1], (float)block[0], (int)gridSize, color),
				cMovable(true),
				cLifetime(30)
			);
		}
		isPause = true;
		isEnd = true;
	}
}