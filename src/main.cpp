#include <Game.h>
#include<UI/UI.h>

const unsigned int WINDOW_START_WIDTH = 1280u;
const unsigned int WINDOW_START_HEIGHT = 720u;

const unsigned int WINDOW_MIN_WIDTH = 400u;
const unsigned int WINDOW_MIN_HEIGHT = 650u;

int main()
{
	Game game;

	//UI ui(game, "Go Game");

	//ui.run();

	//Use console to debug the logic, can be removed
	game.start();
	game.placeStone(1, 1);
	game.pass();
	game.placeStone(2, 2);
	game.pass();
	game.placeStone(2, 0);
	game.pass();
	game.placeStone(3, 1);
	game.print();

	game.placeStone(1, 2);
	game.pass();
	game.placeStone(2, 3);
	game.pass();
	game.placeStone(3, 2);
	game.pass();
	game.print();
	game.placeStone(2, 1);
	game.print();

	game.placeStone(2, 2);
	game.print();

	game.pass();
	game.pass();
	return 0;
}