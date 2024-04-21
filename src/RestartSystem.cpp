#include "RestartSystem.h"


RestartSystem::RestartSystem() {}


void RestartSystem::restart(std::shared_ptr<EntityManager> entityManager , bool& isRestart, bool& isPause, bool& isEnd, bool& spawn)
{
	if (isRestart)
	{
		isRestart = false;
		spawn = true;
		entityManager->clear();
	}

}