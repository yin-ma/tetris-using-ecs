#pragma once
#include "SFML/Graphics.hpp"

struct cPosition
{
	int row = 0;
	int col = 0;
	int oringinRow = 0;
	int oringinCol = 0;

	cPosition();

	cPosition(int inR, int inC, int inOR, int inOC);
};


struct cShape
{
	sf::RectangleShape shape;
	float gridSize = 0;

	cShape();

	cShape(float inX, float inY, float size, sf::Color c);

	void moveTo(char c);
};


struct cMovable
{
	bool isMovable = false;

	cMovable();

	cMovable(bool isM);
};


struct cLifetime
{
	int reminingTime = 0;
	int resetTotalTime = 0;

	cLifetime();

	cLifetime(int inT);
};



struct Action
{
	Action();

	int downKeyLock = 0;
	int leftKeyLock = 0;
	int upKeyLock = 0;
	int rightKeyLock = 0;

	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool r = false;
	bool t = false;
	bool p = false;
};