#include "LifetimeSystem.h"
#include <iostream>

LifetimeSystem::LifetimeSystem() {}


void LifetimeSystem::update(std::shared_ptr<EntityManager> entityManager, Action& action, bool isPause)
{
	if (!isPause)
	{
		std::vector<cMovable>& movableVec = entityManager->getBlockComponentVec<cMovable>();
		std::vector<cLifetime>& lifetimeVec = entityManager->getBlockComponentVec<cLifetime>();
		// lifetime-- for each frame
		for (size_t i = 0; i < movableVec.size(); i++)
		{
			if (movableVec[i].isMovable)
			{
				lifetimeVec[i].reminingTime--;
			}
		}

		action.downKeyLock--;
		action.upKeyLock--;
		action.leftKeyLock--;
		action.rightKeyLock--;
	}
}
