#pragma once
#include <memory>
#include "SFML/Graphics.hpp"
#include "EntityManager.h"


class ClearlineSystem
{
public:
	ClearlineSystem();

	void update(
		std::shared_ptr<EntityManager> entityManager,
		std::shared_ptr<sf::RenderWindow> window,
		int nTileRow,
		int nTileCol,
		bool& islock
	);
};