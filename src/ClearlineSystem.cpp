#include "ClearlineSystem.h"


ClearlineSystem::ClearlineSystem() {}


void ClearlineSystem::update(
	std::shared_ptr<EntityManager> entityManager,
	std::shared_ptr<sf::RenderWindow> window,
	int nTileRow,
	int nTileCol,
	bool& islock
)
{
	if (!islock) return;

	std::vector<int> rowNum(nTileRow, 0);
	std::vector<int> reduceRow(nTileRow, 0);
	int count = 0;

	std::vector<cMovable>& movableVec = entityManager->getBlockComponentVec<cMovable>();
	std::vector<cPosition>& positionVec = entityManager->getBlockComponentVec<cPosition>();
	std::vector<cShape>& shapeVec = entityManager->getBlockComponentVec<cShape>();


	// record which row is full by counting how many blocks on that row
	for (size_t i = 0; i < movableVec.size(); i++)
	{
		if (!movableVec[i].isMovable)
		{
			rowNum[positionVec[i].row] += 1;
		}
	}

	// perform the clear step.
	for (int i = movableVec.size() - 1; i >= 0; i--)
	{
		if (!movableVec[i].isMovable)
		{
			int rowBlock = positionVec[i].row;
			if (rowNum[rowBlock] == nTileCol)
			{
				entityManager->removeEntity(entityManager->getIDs()[i]);
			}
		}
	}

	// move the remaining rows
	for (int i = static_cast<int>(rowNum.size()) - 1; i >= 0; i--)
	{
		reduceRow[i] = count;
		if (rowNum[i] == nTileCol)
		{
			count++;
		}
	}

	for (int i = 0; i < movableVec.size(); i++)
	{
		if (!movableVec[i].isMovable)
		{
			int rowBlock = positionVec[i].row;
			positionVec[i].row += reduceRow[rowBlock];
			for (int n = 0; n < reduceRow[rowBlock]; n++)
			{
				shapeVec[i].moveTo('D');
			}
		}
	}

	islock = false;
}