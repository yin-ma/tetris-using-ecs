#pragma once
#include <memory>
#include "EntityManager.h"


class MovementSystem
{
public:
	MovementSystem();

	void update(std::shared_ptr<EntityManager> entityManager, Action& action, bool& toSpawn, bool& toLock, int nTileRow, int nTileCol, int gridSize, int keyLockDuration);

	bool isValidMove(std::shared_ptr<EntityManager> entityManager, char d, int nTileRow, int nTileCol);
	void moveBlock(std::shared_ptr<EntityManager> entityManager, char d, int gridSize);

	static sf::Vector2f rotatePos(float inX, float inY, float oX, float oY);
	bool isRotateValid(std::shared_ptr<EntityManager> entityManager, int nTileRow, int nTileCol);
	void rotateMovableBlock(std::shared_ptr<EntityManager> entityManager, int nTileRow, int nTileCol, int gridSize);
};