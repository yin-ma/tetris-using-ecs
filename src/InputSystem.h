#pragma once
#include "SFML/Graphics.hpp"
#include "Component.h"



class InputSystem
{
public:
	InputSystem();

	void update(std::shared_ptr<sf::RenderWindow> window, Action& action, int& motionLockTime, bool& isPause, bool& debugMode, bool& isEnd, bool& restart);
};