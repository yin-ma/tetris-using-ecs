#pragma once
#include "SFML/Graphics.hpp"
#include "EntityManager.h"


static const int nTileRow = 20;
static const int nTileCol = 10;
static const int gridSize = 30;


class Game
{
public:
	Game();

	void run();

private:
	sf::RenderWindow m_window;
	const int m_nTileRow = nTileRow;
	const int m_nTileCol = nTileCol;
	const int m_gridSize = gridSize;
	sf::Font m_font;
	int m_debugMode = false;
	int m_lifetime = 30;
	bool m_isEnd = false;
	bool m_isPause = false;
	bool m_moveDown = false;

	std::shared_ptr<EntityManager> m_entityManager;
	std::vector<int> m_entities;
	std::vector<std::vector<int>> m_currentBlock;

	bool isSpawnVaild(std::vector<std::vector<int>>& c);
	void spawnRandomBlock();
	void spawnBlock(char c);

	void moveBlock(char d);
	bool isValidMove(char d);

	static sf::Vector2f rotatePos(float inX, float inY, float oX, float oY);
	bool isRotateValid();
	void rotateMovableBlock();

	void sLifeTime();
	void sMovement();
	void sInput();
	void sClearLine();
	void sRender();

	void reset();
};