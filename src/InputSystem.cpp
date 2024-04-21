#include "InputSystem.h"


InputSystem::InputSystem() {}


void InputSystem::update(std::shared_ptr<sf::RenderWindow> window, Action& action, int& motionLockTime, bool& isPause, bool& debugMode, bool& isEnd, bool& restart)
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window->close();

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::P:
				isPause = !isPause;
				break;
			case sf::Keyboard::T:
				debugMode = !debugMode;
				break;
			case sf::Keyboard::R:
				restart = true;
				isPause = fasle;
				break;
			case sf::Keyboard::W:
			case sf::Keyboard::Up:
				action.up = true;
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				action.left = true;
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				action.right = true;
				break;
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				if (motionLockTime < 0)
				{
					action.down = true;
				}
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
			case sf::Keyboard::Up:
				action.up = false;
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				action.left = false;
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				action.right = false;
				break;
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				action.down = false;
				break;
			}
		}
	}

	motionLockTime--;
}