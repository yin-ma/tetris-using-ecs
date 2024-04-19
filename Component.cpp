#include "Component.h"


cPosition::cPosition() {}

cPosition::cPosition(int r, int c, int oR, int oC)
	: row(r), col(c), oringinRow(oR), oringinCol(oC) {}


cShape::cShape() {}

cShape::cShape(float inX, float inY, float size, sf::Color c)
	:shape(sf::Vector2f(size, size)), gridSize(size)
{
	shape.setPosition(sf::Vector2f(inX*size, inY*size));
	shape.setFillColor(c);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(-1);
}

void cShape::moveTo(char d)
{
	float x = shape.getPosition().x;
	float y = shape.getPosition().y;
	switch (d)
	{
	case 'D':
		shape.setPosition(sf::Vector2f(x, y + gridSize));
	default:
		break;
	}
}


cMovable::cMovable() {}

cMovable::cMovable(bool isM) 
	: isMovable(isM) {}


cLifetime::cLifetime() {}

cLifetime::cLifetime(int inT)
	: t(inT) {}