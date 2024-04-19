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
	m_window.create(sf::VideoMode(m_nTileCol * m_gridSize, m_nTileRow * m_gridSize), "tetris");
	m_window.setFramerateLimit(60);
	m_font.loadFromFile("./TimesNewRoman.ttf");
	m_entityManager = std::make_shared<EntityManager>(m_nTileRow, m_nTileCol, m_gridSize, m_font);

	spawnRandomBlock();
}


void Game::run() 
{
	while (m_window.isOpen())
	{
		sLifeTime();
		sMovement();
		sInput();
		sRender();
	}
}


void Game::moveBlock(char d)
{
	if (m_isPause) return;

	int dRow = 0;
	int dCol = 0;
	if (!isValidMove(d)) return;

	switch (d)
	{
	case 'L':
		{
			dCol -= 1;
		}
		break;
	case 'R':
		{
			dCol = 1;
		}
		break;
	case 'D':
		if (isValidMove('D'))
		{
			dRow = 1;
		}
		break;
	default:
		break;
	}

	// for every movable blocks, perform the move
	for (size_t i = 0; i < m_entityManager->getBlockComponentVec<cMovable>().size(); i++)
	{
		if (m_entityManager->getBlockComponentVec<cMovable>()[i].isMovable)
		{
			float shapeX = m_entityManager->getBlockComponentVec<cShape>()[i].shape.getPosition().x;
			float shapeY = m_entityManager->getBlockComponentVec<cShape>()[i].shape.getPosition().y;

			m_entityManager->getBlockComponentVec<cPosition>()[i].row += dRow;
			m_entityManager->getBlockComponentVec<cPosition>()[i].col += dCol;
			m_entityManager->getBlockComponentVec<cPosition>()[i].oringinRow += dRow;
			m_entityManager->getBlockComponentVec<cPosition>()[i].oringinCol += dCol;
			m_entityManager->getBlockComponentVec<cShape>()[i].shape.setPosition(sf::Vector2f(shapeX + dCol * m_gridSize, shapeY + dRow * m_gridSize));
		}
	}
}


bool Game::isValidMove(char d)
{
	int numEntities = m_entityManager->getBlockComponentVec<cMovable>().size();
	std::vector<cPosition>& positionVec = m_entityManager->getBlockComponentVec<cPosition>();
	std::vector<cMovable>& movableVec = m_entityManager->getBlockComponentVec<cMovable>();
	switch (d)
	{
	case 'D':
		for (size_t i = 0; i < numEntities; i++)
		{
			// check if movable block reach bottom
			if (positionVec[i].row == m_nTileRow - 1 && movableVec[i].isMovable)
			{
				return false;
			}

			// check if movable block collide with other blocks
			for (size_t j = 0; j < numEntities; j++)
			{
				if (movableVec[i].isMovable	&& !movableVec[j].isMovable)
				{
					// if block cannot move downward
					if (positionVec[i].col == positionVec[j].col && positionVec[i].row + 1 == positionVec[j].row)
					{
						return false;
					}
				}
			}

		}
		break;
	case 'L':
		for (size_t i = 0; i < numEntities; i++)
		{
			// check if movable block reach left boundary
			if (positionVec[i].col == 0	&& movableVec[i].isMovable)
			{
				return false;
			}

			// check if movable block collide with other blocks
			for (size_t j = 0; j < numEntities; j++)
			{
				if (movableVec[i].isMovable	&& !movableVec[j].isMovable)
				{
					// if block cannot move left
					if (positionVec[i].col-1 == positionVec[j].col
						&& positionVec[i].row == positionVec[j].row
						)
					{
						return false;
					}
				}
			}

		}
		break;
	case 'R':
		for (size_t i = 0; i < numEntities; i++)
		{
			// check if movable block reach right boundary
			if (positionVec[i].col == m_nTileCol-1 && movableVec[i].isMovable)
			{
				return false;
			}

			// check if movable block collide with other blocks
			for (size_t j = 0; j < numEntities; j++)
			{
				if (movableVec[i].isMovable && !movableVec[j].isMovable)
				{
					// if block cannot move right
					if (positionVec[i].col + 1 == positionVec[j].col && positionVec[i].row == positionVec[j].row)
					{
						return false;
					}
				}
			}
		}
		break;
	default:
		break;
	}
	return true;
}


bool Game::isRotateValid()
{
	// for every block,
	// if the block is movable, 
	// check if block will out of bound
	// and check if any other not movable block collide after rotataion.
	std::vector<cMovable>& movableVec = m_entityManager->getBlockComponentVec<cMovable>();
	std::vector<cPosition>& positionVec = m_entityManager->getBlockComponentVec<cPosition>();
	for (size_t i = 0; i < movableVec.size(); i++)
	{
		if (movableVec[i].isMovable)
		{
			float y = positionVec[i].row + 0.5;
			float x = positionVec[i].col + 0.5;
			float oY = positionVec[i].oringinRow;
			float oX = positionVec[i].oringinCol;

			sf::Vector2f temp = rotatePos(x, y, oX, oY);
			int newRow = (int)(temp.y - 0.5);
			int newCol = (int)(temp.x - 0.5);

			if (newRow < 0 || newRow >= m_nTileRow || newCol < 0 || newCol >= m_nTileCol)
			{
				return false;
			}

			for (size_t j = 0; j < movableVec.size(); j++)
			{
				if (!movableVec[j].isMovable)
				{
					if (newRow == positionVec[j].row && newCol == positionVec[j].col)
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}


sf::Vector2f Game::rotatePos(float inX, float inY, float oX, float oY)
{
	// given a point p(x, y) and origin point o(o1, o2), 
	// return the vector rotate above point o anti-clockwise 90 deg.
	float relativeX = inX - oX;
	float relativeY = inY - oY;

	return sf::Vector2f(relativeY + oX, -relativeX + oY);
}


void Game::rotateMovableBlock()
{
	std::vector<cMovable>& movableVec = m_entityManager->getBlockComponentVec<cMovable>();
	std::vector<cPosition>& positionVec = m_entityManager->getBlockComponentVec<cPosition>();
	std::vector<cShape>& shapeVec = m_entityManager->getBlockComponentVec<cShape>();
	if (!m_isPause && isRotateValid())
	{
		for (size_t i = 0; i < movableVec.size(); i++)
		{
			if (movableVec[i].isMovable)
			{
				// compute the rotated coor
				auto& b = positionVec[i];
				float y = b.row + 0.5;
				float x = b.col + 0.5;
				float oY = b.oringinRow;
				float oX = b.oringinCol;

				sf::Vector2f temp = rotatePos(x, y, oX, oY);
				int newRow = (int)(temp.y - 0.5);
				int newCol = (int)(temp.x - 0.5);
				b.row = newRow;
				b.col = newCol;

				// apply the change
				shapeVec[i].shape.setPosition(sf::Vector2f(newCol*m_gridSize, newRow*m_gridSize));
			}
		}
	}
}


void Game::sLifeTime()
{
	if (!m_isPause)
	{
		std::vector<cMovable>& movableVec = m_entityManager->getBlockComponentVec<cMovable>();
		std::vector<cLifetime>& lifetimeVec = m_entityManager->getBlockComponentVec<cLifetime>();
		// lifetime-- for each frame
		for (size_t i = 0; i < movableVec.size(); i++)
		{
			if (movableVec[i].isMovable)
			{
				lifetimeVec[i].t -= 1;
			}
		}		
	}
}


void Game::sMovement()
{
	// check if the downward move is valid.
	// and check if lifetime is <0. if both true, then move the block in y direction. (i.e. ++y)
	// if false lock the block and change it to not movable.
	std::vector<cMovable>& movableVec = m_entityManager->getBlockComponentVec<cMovable>();
	std::vector<cLifetime>& lifetimeVec = m_entityManager->getBlockComponentVec<cLifetime>();
	std::vector<cShape>& shapeVec = m_entityManager->getBlockComponentVec<cShape>();
	std::vector<cPosition>& positionVec = m_entityManager->getBlockComponentVec<cPosition>();
	
	if (isValidMove('D'))
	{
		for (size_t i = 0; i < movableVec.size(); i++)
		{
			if (movableVec[i].isMovable	&& lifetimeVec[i].t <= 0)
			{
				lifetimeVec[i].t = m_lifetime;
				shapeVec[i].moveTo('D');
				positionVec[i].row += 1;
				positionVec[i].oringinRow += 1;
			}
		}
	} 
	else
	{
		bool spawned = false;
		// lock the moving blocks when lifetime is up
		for (size_t i = 0; i < movableVec.size(); i++)
		{
			if (movableVec[i].isMovable && lifetimeVec[i].t <= 0)
			{
				movableVec[i].isMovable = false;

				if (!spawned)
				{
					spawnRandomBlock();
					spawned = true;
				}
			}
		}

		sClearLine();

	}
}


void Game::sInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_window.close();

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::P:
				m_isPause = !m_isPause;
				break;
			case sf::Keyboard::T:
				m_debugMode = ~m_debugMode;
				break;
			case sf::Keyboard::R:
				reset();
				break;
			case sf::Keyboard::W:
			case sf::Keyboard::Up:
				rotateMovableBlock();
				break;
			case sf::Keyboard::A:
			case sf::Keyboard::Left:
				moveBlock('L');
				break;
			case sf::Keyboard::D:
			case sf::Keyboard::Right:
				moveBlock('R');
				break;
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				m_moveDown = true;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::S:
			case sf::Keyboard::Down:
				m_moveDown = false;
				break;
			default:
				break;
			}
		}
	}

	if (m_moveDown)
	{
		moveBlock('D');
	}
}


void Game::sClearLine()
{
	std::vector<int> rowNum(m_nTileRow, 0);
	std::vector<int> reduceRow(m_nTileRow, 0);
	int count = 0;

	std::vector<cMovable>& movableVec = m_entityManager->getBlockComponentVec<cMovable>();
	std::vector<cPosition>& positionVec = m_entityManager->getBlockComponentVec<cPosition>();
	std::vector<cShape>& shapeVec = m_entityManager->getBlockComponentVec<cShape>();


	// record which row is full by counting how many blocks on that row
	for (size_t i = 0; i < movableVec.size(); i++)
	{
		if (!movableVec[i].isMovable)
		{
			rowNum[positionVec[i].row] += 1;
		}
	}

	// perform the clear step.
	for (int i = movableVec.size() - 1; i >= 0; i--)
	{
		if (!movableVec[i].isMovable)
		{
			int rowBlock = positionVec[i].row;
			if (rowNum[rowBlock] == m_nTileCol)
			{
				m_entityManager->removeEntity(m_entityManager->getIDs()[i]);
			}
		}
	}

	// move the remaining rows
	for (int i = static_cast<int>(rowNum.size()) - 1; i >= 0; i--)
	{
		reduceRow[i] = count;
		if (rowNum[i] == m_nTileCol)
		{
			count++;
		}
	}

	for (int i = 0; i < movableVec.size(); i++)
	{
		if (!movableVec[i].isMovable)
		{
			int rowBlock = positionVec[i].row;
			positionVec[i].row += reduceRow[rowBlock];
			for (int n = 0; n < reduceRow[rowBlock]; n++)
			{
				shapeVec[i].moveTo('D');
			}
		}
	}
}


void Game::sRender()
{
	m_window.clear(sf::Color(0, 0, 0));

	for (auto& e : m_entityManager->getBlockComponentVec<cShape>())
	{
		m_window.draw(e.shape);
	}

	if (m_debugMode)
	{
		for (auto& s : m_entityManager->getBoardComponentVec<sf::RectangleShape>())
		{
			m_window.draw(s);
		}

		for (auto& t : m_entityManager->getBoardComponentVec<sf::Text>())
		{
			m_window.draw(t);
		}
	}

	if (m_isPause && !m_isEnd)
	{
		auto text = sf::Text::Text();
		text.setFont(m_font);
		text.setString("Pause");
		text.setFillColor(sf::Color::White);
		text.setCharacterSize(40);

		float midX = (m_window.getSize().x - text.getGlobalBounds().width) / 2;
		float midY = (m_window.getSize().y - text.getGlobalBounds().height) / 2;
		text.setPosition(sf::Vector2f(midX, midY));

		m_window.draw(text);
	}

	if (m_isEnd && m_isPause)
	{
		if (m_isPause)
		{
			auto text = sf::Text::Text();
			text.setFont(m_font);
			text.setString("Press R to restart");
			text.setFillColor(sf::Color::White);
			text.setCharacterSize(40);

			float midX = (m_window.getSize().x - text.getGlobalBounds().width) / 2;
			float midY = (m_window.getSize().y - text.getGlobalBounds().height) / 2;
			text.setPosition(sf::Vector2f(midX, midY));

			m_window.draw(text);
		}
	}
	m_window.display();
}


bool Game::isSpawnVaild(std::vector<std::vector<int>>& c)
{
	for (std::vector<int> point : c)
	{
		int row = point[0];
		int col = point[1];

		for (auto& b : m_entityManager->getBlockComponentVec<cPosition>())
		{
			if (row == b.row && col == b.col)
			{
				return false;
			}
		}
	}
	return true;
}


void Game::spawnRandomBlock()
{
	spawnBlock(blockArr[std::rand() % 7]);
}


void Game::spawnBlock(char c)
{
	int row = 0;
	int col = (int)m_nTileCol / 2 - 1;
	int oRow = 0;
	int oCol = 0;
	std::vector<std::vector<int>> shape;
	sf::Color color;
	switch (c)
	{
	case 'O':
		shape = {
			{0, col}, {0, col + 1},
			{1, col}, {1, col + 1}
		};
		color = sf::Color::Red;
		oRow = 1;
		oCol = col+1;
		break;
	case 'L':
		shape = {
			{0, col - 1}, {1, col - 1},
			{1, col}, {1, col + 1}
		};
		oRow = 1;
		oCol = col;
		color = sf::Color::Green;
		break;
	case 'I':
		shape = {
			{0, col}, {1, col},
			{2, col}, {3, col}
		};
		color = sf::Color::Blue;
		oRow = 2;
		oCol = col;
		break;
	case 'N':
		shape = {
			{0, col - 1}, {0, col},
			{1, col}, {1, col+1}
		};
		color = sf::Color::Cyan;
		oRow = 1;
		oCol = col + 1;
		break;
	case 'Z':
		shape = {
			{0, col}, {0, col + 1},
			{1, col - 1}, {1, col}
		};
		color = sf::Color::Yellow;
		oRow = 1;
		oCol = col;
		break;
	case 'T':
		shape = {
			{0, col - 1}, {0, col},
			{0, col + 1}, {1, col}
		};
		color = sf::Color(245, 141, 66);
		oRow = 1;
		oCol = col+1;
		break;
	case 'J':
		shape = {
			{0, col-1}, {0, col},
			{0, col+1}, {1, col-1}
		};
		oRow = 1;
		oCol = col;
		color = sf::Color(66, 141, 245);
		break;
	default:
		break;
	}


	if (isSpawnVaild(shape))
	{
		// gen new block and add it to entityManager
		for (auto& block : shape)
		{
			m_entityManager->addEntity(
				cPosition(block[0], block[1], oRow, oCol),
				cShape((float)block[1], (float)block[0], (float)m_gridSize, color),
				cMovable(true),
				cLifetime(m_lifetime)
			);
		}
	}
	else
	{
		for (auto& block : shape)
		{
			m_entityManager->addEntity(
				cPosition(block[0], block[1], oRow, oCol),
				cShape((float)block[1], (float)block[0], (float)m_gridSize, color),
				cMovable(true),
				cLifetime(30)
			);
		}
		m_isPause = true;
		m_isEnd = true;
	}


}


void Game::reset()
{
	m_isPause = false;
	m_isEnd = false;
	m_entityManager->clear();
	spawnRandomBlock();
}