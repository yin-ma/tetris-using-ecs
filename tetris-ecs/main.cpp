#include "Game.h"


// TODO:
// #1.scoring system / level system
// #2.action lock after clear line


int main()
{
	srand(time(NULL));
	Game game;
	game.run();
	return 0;
}