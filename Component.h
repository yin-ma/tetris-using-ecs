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
	int t = 0;

	cLifetime();

	cLifetime(int inT);
};