#pragma once
#include "SFML/Graphics.hpp"
#include "Component.h"
#include "EntityManager.h"
#include <memory>


class RestartSystem
{
public:
	RestartSystem();

	void restart(std::shared_ptr<EntityManager> entityManager, bool& isRestart, bool& spawn);
};