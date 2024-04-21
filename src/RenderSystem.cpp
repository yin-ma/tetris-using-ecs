#pragma once

#include "EntityManager.h"
#include "RenderSystem.h"
#include "Component.h"



RenderSystem::RenderSystem() {}


void RenderSystem::update(
	std::shared_ptr<EntityManager> entityManager,
	std::shared_ptr<sf::RenderWindow> window,
	sf::Font& font,
	bool debugMode,
	bool isPause,
	bool isEnd
)
{
	window->clear(sf::Color(0, 0, 0));

	for (auto& e : entityManager->getBlockComponentVec<cShape>())
	{
		window->draw(e.shape);
	}

	if (debugMode)
	{
		for (auto& s : entityManager->getBoardComponentVec<sf::RectangleShape>())
		{
			window->draw(s);
		}

		for (auto& t : entityManager->getBoardComponentVec<sf::Text>())
		{
			window->draw(t);
		}
	}

	if (isPause && !isEnd)
	{
		auto text = sf::Text::Text();
		text.setFont(font);
		text.setString("Pause");
		text.setFillColor(sf::Color::White);
		text.setCharacterSize(40);

		float midX = (window->getSize().x - text.getGlobalBounds().width) / 2;
		float midY = (window->getSize().y - text.getGlobalBounds().height) / 2;
		text.setPosition(sf::Vector2f(midX, midY));

		window->draw(text);
	}

	if (isEnd && isPause)
	{
		auto text = sf::Text::Text();
		text.setFont(font);
		text.setString("Press R to restart");
		text.setFillColor(sf::Color::White);
		text.setCharacterSize(40);

		float midX = (window->getSize().x - text.getGlobalBounds().width) / 2;
		float midY = (window->getSize().y - text.getGlobalBounds().height) / 2;
		text.setPosition(sf::Vector2f(midX, midY));

		window->draw(text);
	}
	window->display();
}