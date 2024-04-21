#include <iostream>
#include <memory>
#include <string>
#include <ctime>

#include <SFML/Graphics.hpp>

#include "Game.h"


static char blockArr[] = { 'O', 'L', 'I', 'Z', 'N', 'T', 'J' };


Game::Game()
{
	// init
	m_window = std::make_shared<sf::RenderWindow>(
		sf::VideoMode(m_nTileCol * m_gridSize, m_nTileRow * m_gridSize), "tetris"
	);
	m_window->setFramerateLimit(60);
	m_font.loadFromFile("assets/TimesNewRoman.ttf");
	m_entityManager = std::make_shared<EntityManager>(m_nTileRow, m_nTileCol, m_gridSize, m_font);
}


void Game::run() 
{
	while (m_window->isOpen())
	{
		m_lifetimeSystem.update(m_entityManager, m_action, m_isPause);
		m_movementSystem.update(m_entityManager, m_action, m_spawn, m_islock, m_nTileRow, m_nTileCol, m_gridSize, m_keyLockDuration);
		m_inputSystem.update(m_window, m_action, m_motionLockTime, m_isPause, m_debugMode, m_isEnd, m_restart);
		m_clearlineSystem.update(m_entityManager, m_window, m_nTileRow, m_nTileCol, m_islock);
		m_renderSystem.update(m_entityManager, m_window, m_font, m_debugMode, m_isPause, m_isEnd);
		m_spawnSystem.update(m_entityManager, m_nTileRow, m_nTileCol, m_gridSize, m_lifetime, m_spawn, m_isPause, m_isEnd);
		m_restartSystem.restart(m_entityManager, m_restart, m_isPause, m_isEnd, m_spawn);
	}
}