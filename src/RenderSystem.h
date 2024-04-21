#pragma once

#include "EntityManager.h"
#include "SFML/Graphics.hpp"


class RenderSystem
{
public:
	RenderSystem();

	void update(
		std::shared_ptr<EntityManager> entityManager,
		std::shared_ptr<sf::RenderWindow> window,
		sf::Font& font,
		bool isDebugMode,
		bool isPause,
		bool isEnd
	);
};