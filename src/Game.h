#pragma once
#include "SFML/Graphics.hpp"
#include "EntityManager.h"
#include "RenderSystem.h"
#include "ClearlineSystem.h"
#include "LifetimeSystem.h"
#include "MovementSystem.h"
#include "SpawnSystem.h"
#include "InputSystem.h"
#include "RestartSystem.h"


static const int nTileRow = 20;
static const int nTileCol = 10;
static const int gridSize = 30;
static int lifetime = 30;


class Game
{
public:
	Game();

	void run();

private:
	std::shared_ptr<sf::RenderWindow> m_window;
	std::shared_ptr<EntityManager> m_entityManager;
	sf::Font m_font;

	const int m_nTileRow = nTileRow;
	const int m_nTileCol = nTileCol;
	const int m_gridSize = gridSize;
	Action m_action;
	int m_lifetime = lifetime;
	int m_keyLockDuration = 4;
	int m_motionLockTime = 5;
	bool m_debugMode = false;
	bool m_isEnd = false;
	bool m_isPause = false;
	bool m_islock = false;
	bool m_spawn = true;
	bool m_restart = false;

	SpawnSystem m_spawnSystem;
	RenderSystem m_renderSystem;
	ClearlineSystem m_clearlineSystem;
	LifetimeSystem m_lifetimeSystem;
	MovementSystem m_movementSystem;
	InputSystem m_inputSystem;
	RestartSystem m_restartSystem;
};