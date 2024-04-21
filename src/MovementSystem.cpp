#include "MovementSystem.h"
#include "Component.h"
#include <iostream>
#include <cmath>


MovementSystem::MovementSystem() {}


void MovementSystem::update(std::shared_ptr<EntityManager> entityManager, Action& action, bool& toSpawn, bool& toLock, int nTileRow, int nTileCol, int gridSize, int keyLockDuration)
{
	std::vector<cMovable>& movableVec = entityManager->getBlockComponentVec<cMovable>();
	std::vector<cLifetime>& lifetimeVec = entityManager->getBlockComponentVec<cLifetime>();
	std::vector<cShape>& shapeVec = entityManager->getBlockComponentVec<cShape>();
	std::vector<cPosition>& positionVec = entityManager->getBlockComponentVec<cPosition>();

	if (isValidMove(entityManager, 'D', nTileRow, nTileCol))
	{
		for (size_t i = 0; i < movableVec.size(); i++)
		{
			if (movableVec[i].isMovable && lifetimeVec[i].reminingTime <= 0)
			{
				lifetimeVec[i].reminingTime = lifetimeVec[i].resetTotalTime;
				shapeVec[i].moveTo('D');
				positionVec[i].row += 1;
				positionVec[i].oringinRow += 1;
			}
		}
	}
	else
	{
		bool spawned = false;
		// lock the moving blocks when lifetime is up
		for (size_t i = 0; i < movableVec.size(); i++)
		{
			if (movableVec[i].isMovable && lifetimeVec[i].reminingTime <= 0)
			{
				movableVec[i].isMovable = false;

				if (!spawned)
				{
					toSpawn = true;
					spawned = true;
					toLock = true;
				}
			}
		}
		action.downKeyLock = 5;
		action.down = false;
		//m_islock = true;
	}

	if (action.up && action.upKeyLock < 0 && isRotateValid(entityManager, nTileRow, nTileCol)) 
	{ 
		rotateMovableBlock(entityManager, nTileRow, nTileCol, gridSize); 
		action.upKeyLock = 7;
	}
	if (action.left && action.leftKeyLock < 0 && isValidMove(entityManager, 'L', nTileRow, nTileCol))
	{ 
		moveBlock(entityManager, 'L', gridSize); 
		action.leftKeyLock = keyLockDuration;
	}
	if (action.right && action.rightKeyLock < 0 && isValidMove(entityManager, 'R', nTileRow, nTileCol))
	{	
		moveBlock(entityManager, 'R', gridSize); 
		action.rightKeyLock = keyLockDuration;
	}
	if (action.down && action.downKeyLock < 0 && isValidMove(entityManager, 'D', nTileRow, nTileCol))
	{ 
		moveBlock(entityManager, 'D', gridSize);
		action.downKeyLock = 1;
	}
}


bool MovementSystem::isValidMove(std::shared_ptr<EntityManager> entityManager, char d, int nTileRow, int nTileCol)
{
	int numEntities = entityManager->getBlockComponentVec<cMovable>().size();
	std::vector<cPosition>& positionVec = entityManager->getBlockComponentVec<cPosition>();
	std::vector<cMovable>& movableVec = entityManager->getBlockComponentVec<cMovable>();
	switch (d)
	{
	case 'D':
		for (size_t i = 0; i < numEntities; i++)
		{
			// check if movable block reach bottom
			if (positionVec[i].row == nTileRow - 1 && movableVec[i].isMovable)
			{
				return false;
			}

			// check if movable block collide with other blocks
			for (size_t j = 0; j < numEntities; j++)
			{
				if (movableVec[i].isMovable && !movableVec[j].isMovable)
				{
					// if block cannot move downward
					if (positionVec[i].col == positionVec[j].col && positionVec[i].row + 1 == positionVec[j].row)
					{
						return false;
					}
				}
			}

		}
		break;
	case 'L':
		for (size_t i = 0; i < numEntities; i++)
		{
			// check if movable block reach left boundary
			if (positionVec[i].col == 0 && movableVec[i].isMovable)
			{
				return false;
			}

			// check if movable block collide with other blocks
			for (size_t j = 0; j < numEntities; j++)
			{
				if (movableVec[i].isMovable && !movableVec[j].isMovable)
				{
					// if block cannot move left
					if (positionVec[i].col - 1 == positionVec[j].col
						&& positionVec[i].row == positionVec[j].row
						)
					{
						return false;
					}
				}
			}

		}
		break;
	case 'R':
		for (size_t i = 0; i < numEntities; i++)
		{
			// check if movable block reach right boundary
			if (positionVec[i].col == nTileCol - 1 && movableVec[i].isMovable)
			{
				return false;
			}

			// check if movable block collide with other blocks
			for (size_t j = 0; j < numEntities; j++)
			{
				if (movableVec[i].isMovable && !movableVec[j].isMovable)
				{
					// if block cannot move right
					if (positionVec[i].col + 1 == positionVec[j].col && positionVec[i].row == positionVec[j].row)
					{
						return false;
					}
				}
			}
		}
		break;
	default:
		break;
	}
	return true;
}


void MovementSystem::moveBlock(std::shared_ptr<EntityManager> entityManager, char d, int gridSize)
{
	int dRow = 0;
	int dCol = 0;

	switch (d)
	{
	case 'L':
	{
		dCol -= 1;
	}
	break;
	case 'R':
	{
		dCol = 1;
	}
	break;
	case 'D':
		{
			dRow = 1;
		}
		break;
	default:
		break;
	}

	// for every movable blocks, perform the move
	for (size_t i = 0; i < entityManager->getBlockComponentVec<cMovable>().size(); i++)
	{
		if (entityManager->getBlockComponentVec<cMovable>()[i].isMovable)
		{
			float shapeX = entityManager->getBlockComponentVec<cShape>()[i].shape.getPosition().x;
			float shapeY = entityManager->getBlockComponentVec<cShape>()[i].shape.getPosition().y;

			entityManager->getBlockComponentVec<cPosition>()[i].row += dRow;
			entityManager->getBlockComponentVec<cPosition>()[i].col += dCol;
			entityManager->getBlockComponentVec<cPosition>()[i].oringinRow += dRow;
			entityManager->getBlockComponentVec<cPosition>()[i].oringinCol += dCol;
			entityManager->getBlockComponentVec<cShape>()[i].shape.setPosition(sf::Vector2f(shapeX + dCol * gridSize, shapeY + dRow * gridSize));
		}
	}
}


void MovementSystem::rotateMovableBlock(std::shared_ptr<EntityManager> entityManager, int nTileRow, int nTileCol, int gridSize)
{
	std::vector<cMovable>& movableVec = entityManager->getBlockComponentVec<cMovable>();
	std::vector<cPosition>& positionVec = entityManager->getBlockComponentVec<cPosition>();
	std::vector<cShape>& shapeVec = entityManager->getBlockComponentVec<cShape>();
	if (isRotateValid(entityManager, nTileRow, nTileCol))
	{
		for (size_t i = 0; i < movableVec.size(); i++)
		{
			if (movableVec[i].isMovable)
			{
				// compute the rotated coor
				auto& b = positionVec[i];
				float y = b.row + 0.5;
				float x = b.col + 0.5;
				float oY = b.oringinRow;
				float oX = b.oringinCol;

				sf::Vector2f temp = rotatePos(x, y, oX, oY);
				int newRow = (int)(temp.y - 0.5);
				int newCol = (int)(temp.x - 0.5);
				b.row = newRow;
				b.col = newCol;

				// apply the change
				shapeVec[i].shape.setPosition(sf::Vector2f(newCol * gridSize, newRow * gridSize));
			}
		}
	}
}


bool MovementSystem::isRotateValid(std::shared_ptr<EntityManager> entityManager, int nTileRow, int nTileCol)
{
	// for every block,
	// if the block is movable, 
	// check if block will out of bound
	// and check if any other not movable block collide after rotataion.
	std::vector<cMovable>& movableVec = entityManager->getBlockComponentVec<cMovable>();
	std::vector<cPosition>& positionVec = entityManager->getBlockComponentVec<cPosition>();
	for (size_t i = 0; i < movableVec.size(); i++)
	{
		if (movableVec[i].isMovable)
		{
			float y = positionVec[i].row + 0.5;
			float x = positionVec[i].col + 0.5;
			float oY = positionVec[i].oringinRow;
			float oX = positionVec[i].oringinCol;

			sf::Vector2f temp = rotatePos(x, y, oX, oY);
			int newRow = (int)(temp.y - 0.5);
			int newCol = (int)(temp.x - 0.5);

			if (newRow < 0 || newRow >= nTileRow || newCol < 0 || newCol >= nTileCol)
			{
				return false;
			}

			for (size_t j = 0; j < movableVec.size(); j++)
			{
				if (!movableVec[j].isMovable)
				{
					if (newRow == positionVec[j].row && newCol == positionVec[j].col)
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}



sf::Vector2f MovementSystem::rotatePos(float inX, float inY, float oX, float oY)
{
	// given a point p(x, y) and origin point o(o1, o2), 
	// return the vector rotate above point o anti-clockwise 90 deg.
	float relativeX = inX - oX;
	float relativeY = inY - oY;

	return sf::Vector2f(relativeY + oX, -relativeX + oY);
}