#pragma once
#include <memory>
#include "EntityManager.h"


class LifetimeSystem
{
public:
	LifetimeSystem();

	void update(std::shared_ptr<EntityManager> entityManager, Action& action, bool isPause);
};