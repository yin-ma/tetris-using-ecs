#pragma once
#include <vector>
#include "EntityManager.h"
#include "SFML/Graphics.hpp"


class SpawnSystem
{
private:
	const char blockArr[7] = { 'O', 'L', 'I', 'Z', 'N', 'T', 'J' };
public:
	SpawnSystem();

	void update(std::shared_ptr<EntityManager> entityManager, int nTileRow, int nTileCol, int gridSize, int lifetime, bool& spawn, bool& isPause, bool& isEnd);

	bool isSpawnVaild(std::shared_ptr<EntityManager> entityManager, std::vector<std::vector<int>>& c);

	void spawnBlock(char c, std::shared_ptr<EntityManager> entityManager, int nTileRow, int nTileCol, int gridSize, int lifetime, bool& spawn, bool& isPause, bool& isEnd);
};